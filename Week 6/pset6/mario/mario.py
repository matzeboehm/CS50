from cs50 import get_int

# User input
while True:
    number = get_int("Number: ")

    if number >= 1 and number <= 8:
        break

# Print
for i in range(number):
    print((number - i - 1) * " " + (i + 1) * "#")