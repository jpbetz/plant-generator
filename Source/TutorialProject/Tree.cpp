
#include "TutorialProject.h"
#include "Tree.h"

ATree::ATree(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP) {
    
    InstancedTreeComponent = PCIP.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, TEXT("InstancedTreeComponent"));
    InstancedLeafComponent = PCIP.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, TEXT("InstancedLeafComponent"));
}

void ATree::PostInitializeComponents() {
    Super::PostInitializeComponents();
    
    // branches
    {
        InstancedTreeComponent->AttachTo(RootComponent);
        InstancedTreeComponent->SetStaticMesh(TreeStaticMesh);
        InstancedTreeComponent->SetMaterial(0, UMaterialInstanceDynamic::Create(TreeMaterial, this));
        InstancedTreeComponent->BodyInstance.SetObjectType(ECC_WorldDynamic);
        InstancedTreeComponent->SetMobility(EComponentMobility::Movable);
    }
    
    // leaves
    {
        InstancedLeafComponent->AttachTo(RootComponent);
        InstancedLeafComponent->SetStaticMesh(LeafStaticMesh);
        InstancedLeafComponent->SetMaterial(0, UMaterialInstanceDynamic::Create(LeafMaterial, this));
        InstancedLeafComponent->BodyInstance.SetObjectType(ECC_WorldDynamic);
        InstancedLeafComponent->SetMobility(EComponentMobility::Movable);
    }

    InterpretLSystemDerivation();
}

void ATree::InterpretLSystemDerivation() {
    
    // F[\\&&F^^F][//&&F^^F]
    // F[\\++F--F][//++F--F]
    // F//++F//++F//++F//++F
    
    // TODO: add complex type that has both of these fields so I only need a single stack
    TArray<FTransform> stack = TArray<FTransform>();
    TArray<FQuat> rotatorStack = TArray<FQuat>();
    
    // rotation, translation, scale
    FTransform base = FTransform(FRotator(0, 0, 0), FVector(0, 0, 0), FVector(Radius/100, Radius/100, Length/100));
    FTransform relative = FTransform(FRotator(0, 0, 0), FVector(0, 0, 0), FVector(1,1,1));
    
    
    FTransform forward = FTransform(FRotator(0, 0, 0), FVector(0, 0, 100), FVector(0, 0, 0));
    
    // my orientation is facing the Z axis,  so pitch, yaw and roll are not matching up with the FRotator's understanding of them, which is for facing the X axis
    FQuat pitchCW = FQuat(FRotator(Angle, 0, 0));
    FQuat pitchCCW = FQuat(FRotator(-Angle, 0, 0));
    
    FQuat rollCW = FQuat(FRotator(0, Angle, 0));
    FQuat rollCCW = FQuat(FRotator(0, -Angle, 0));
    
    FQuat yawCW = FQuat(FRotator(0, 0, Angle));
    FQuat yawCCW = FQuat(FRotator(0, 0, -Angle));
    
    FQuat reverse = FQuat(FRotator(180, 0, 0)) * FQuat(FRotator(0, 180, 0)); // should rotate 180 on yaw axis...

    FTransform currentGlobal = base;
    FQuat currentRelativeRotator = FQuat(FRotator(0,0,0));
    
    FTransform nextRelative = FTransform(relative);
    
    // TODO:  improve to interpret 3d rotational symbols used by many l-systems
    for(TCHAR c : lSystemDerivation.GetCharArray()) {
        switch(c) {
            case 'F':
                {
                    float branchXY = FMath::Clamp(currentGlobal.GetScale3D().X-(BrancRadiusReduction/100), MinBranchRadius/100, Radius/100);
                    float branchZ = currentGlobal.GetScale3D().Z * BranchLengthRelativeToParent;
                    nextRelative.SetScale3D(FVector(branchXY, branchXY, branchZ));
                    nextRelative.SetRotation(currentRelativeRotator);
                    
                    FTransform nextGlobal = TransformToGlobal(currentGlobal, nextRelative);
                    InstancedTreeComponent->AddInstance(nextGlobal);
                    currentGlobal = nextGlobal;
                    currentRelativeRotator = FQuat(FRotator(0,0,0));
                    nextRelative = FTransform(relative);
                    nextRelative += forward;
                }
                break;
            case 'L':
                {
                    nextRelative.SetScale3D(FVector(LeafSize/100, LeafSize/100, LeafSize/100));
                    nextRelative.SetRotation(currentRelativeRotator);
                    
                    FTransform nextGlobal = TransformToGlobal(currentGlobal, nextRelative);
                    InstancedLeafComponent->AddInstance(nextGlobal);
                    currentGlobal = nextGlobal;
                    currentRelativeRotator = FQuat(FRotator(0,0,0));
                    nextRelative = FTransform(relative);
                }
                break;
            case '[':
                stack.Push(currentGlobal);
                rotatorStack.Push(currentRelativeRotator);
                break;
            case ']':
                currentGlobal = stack.Pop();
                currentRelativeRotator = rotatorStack.Pop();
                break;
            case '&':
                currentRelativeRotator = currentRelativeRotator * pitchCW;
                break;
            case '^':
                currentRelativeRotator = currentRelativeRotator * pitchCCW;
                break;
            case '+':
                currentRelativeRotator = currentRelativeRotator * yawCW;
                break;
            case '-':
                currentRelativeRotator = currentRelativeRotator * yawCCW;
                break;
            case '\\':
                currentRelativeRotator = currentRelativeRotator * rollCW;
                break;
            case '/':
                currentRelativeRotator = currentRelativeRotator * rollCCW;
                break;
            case '|':
                currentRelativeRotator = currentRelativeRotator * reverse;
                break;
            default:
                break;
        }
    }
}

// TODO: is there an existing routine in UE4 that already does this?  There should be.
FTransform ATree::TransformToGlobal(const FTransform parent, const FTransform local) {
    FTransform result = FTransform(local);

    result.SetRotation(parent.GetRotation() * local.GetRotation());
    result.SetTranslation(parent.GetTranslation() + parent.GetRotation().RotateVector(parent.GetScale3D() * local.GetTranslation()));
    result.SetScale3D(/*parent.GetScale3D() **/ local.GetScale3D());
    return result;
}