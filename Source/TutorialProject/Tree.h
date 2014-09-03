

#pragma once

#include <string>

#include "GameFramework/Actor.h"
#include "Tree.generated.h"

/**
 * 
 */
UCLASS() // lets you set various properties and makes the editor aware of the class for stuff like GC
class TUTORIALPROJECT_API ATree : public AActor
{
	GENERATED_UCLASS_BODY()

	// add property declarations here, e.g.:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection)
    FString lSystemDerivation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection)
    float Angle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection)
    float Radius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection)
    float Length;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection)
    float BrancRadiusReduction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection)
    float MinBranchRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection)
    float BranchLengthRelativeToParent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection)
    float LeafSize;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Tree)
    TSubobjectPtr<UInstancedStaticMeshComponent> InstancedTreeComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tree)
    UMaterial* TreeMaterial;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tree)
    UStaticMesh *TreeStaticMesh;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Leaves)
    TSubobjectPtr<UInstancedStaticMeshComponent> InstancedLeafComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Leaves)
    UMaterial* LeafMaterial;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Leaves)
    UStaticMesh *LeafStaticMesh;

    
    // standard unreal engine lifecycle hook
    void PostInitializeComponents();
    
    FTransform TransformToGlobal(FTransform parent, FTransform local);
    
    void InterpretLSystemDerivation();
};
