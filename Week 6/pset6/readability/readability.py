from cs50 import get_string

text = get_string("Text: ")

words = 0
sentences = 0
letters = 0

# check every character
for i in text:
    if i.isalpha() == True:
        letters += 1
    elif i == " ":
        words += 1
    elif i == "." or i == "!" or i == "?":
        sentences += 1
if words != 0:
    words += 1

# formula to calculate the reading level
L = letters / words * 100
S = sentences / words * 100
formula = 0.0588 * L - 0.296 * S - 15.8

# round level to next integer
grade = int(round(formula))

# decision
if grade < 1:
    print("Before Grade 1")
elif grade >= 16:
    print("Grade 16+")
else:
    print(f"Grade {grade}")