import random
import string

input = [""]
halfSize = 1000000

def generateRandomName():
    length = random.randint(3, 18)
    return ''.join(random.choices(string.ascii_lowercase, k=length))

for i in range(0, halfSize):
    j = i % 2

    firstName = generateRandomName()
    lastName = generateRandomName()

    input.append(f"i {i} {firstName} {lastName}\n") # Insert

    if (j == 0): # Search
        input.append(f"s {i}\n")
    if (j == 1): # Delete
        input.append(f"d {i}\n")

random.shuffle(input)

with open(f'Operations/operations2.txt', 'w', encoding='utf-8') as file:
    for item in input:
        file.write(item)
