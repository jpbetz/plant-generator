import sys
import getopt
import pprint

opts, args = getopt.getopt(sys.argv[1:], "n:", [])

n = 1
axiom = ""
rules = {}

for opt, arg in opts:
  if opt == '-n':
    n = int(arg)

for linenum, line in enumerate(sys.stdin):
  line = line.strip()
  #if linenum == 0:
  # axiom = line
  #else:
  parts = line.split(":")
  if len(parts) == 2:
    rules[parts[0]] = parts[1]

if 'axiom' in rules:
  axiom = rules['axiom']
  del rules['axiom']
else:
  print 'axiom missing!,  use "axiom:" to define one'
  sys.exit(1)

predecessor = axiom
successor = axiom

for i in range(1, n+1):
  successor = ""
  for c in predecessor:
    if c in rules:
      successor += rules[c]
    else:
      successor += c
  predecessor = successor

print 'axiom: {}'.format(repr(axiom))
print 'rules:'
for key, value in sorted(rules.items()):
  print("\t{} : {}".format(key, value))
print 'n: {}'.format(n)
print ''
print successor
print ''

trimmed = ""
for c in successor:
  if c in ['F', '[', ']', '\\', '/', '&', '^', '-', '+', '|', 'L']:
    trimmed += c
print trimmed
print ''
