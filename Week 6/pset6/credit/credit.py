from cs50 import get_string

number = get_string("Enter credit card number: ")

if number.isdecimal() and (len(number) == 13 or len(number) == 15 or len(number) == 16):
    odd = 0
    even = 0
    for i in range(1, len(number) + 1):
        # even numbers
        if i % 2 == 0:
            if int(number[-i]) * 2 >= 10:
                lastnum = (int(number[-i]) * 2) % 10
                firstnum = (int(number[-i]) * 2 - lastnum) / 10
                even = even + lastnum + firstnum
            else:
                even = even + int(number[-i]) * 2
        # odd numbers
        else:
            odd = odd + int(number[-i])

    # security number
    security = int(odd + even)
    
    # real card
    if (security % 10 == 0):
        if len(number) == 15 and number[0] == "3" and (number[1] == "4" or number[1] == "7"):
            print("AMEX")
        elif len(number) == 16 and number[0] == "5" and (number[1] == "1" or number[1] == "2" or number[1] == "3" or number[1] == "4" or number[1] == "5"):
            print("MASTERCARD")
        elif len(number) == 16 and number[0] == "4":
            print("VISA")
        else:
            print("INVALID")
        
    # fake card
    else:
        print("INVALID")
    
else:
    print("INVALID")