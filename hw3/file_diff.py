import os

parent_dir = './jhinput'
f1 = 'scheduler.txt'
f2 = 'scheduler_jaeun.txt'

with open(os.path.join(parent_dir, f1)) as f:
    str1 = f.readlines()

with open(os.path.join(parent_dir, f2)) as f:
    str2 = f.readlines()

assert len(str1) == len(str2)

cnt = 0
for i in range(len(str1)):
    if str1[i].strip().replace(' ', '').replace(':', '') != str2[i].strip().replace(' ', '').replace(':', ''):
        cnt += 1
        j = i
        while j >= 0:
            if 'Cycle' in str1[j]:
                break
            j -= 1
        cycle = int(str1[j].split(' ')[0][1:])
        print("Cycle: %d" % cycle)
        print(str1[i].strip())
        print(str2[i].strip())
        print()

print("Total diff cnt: %d" % cnt)
