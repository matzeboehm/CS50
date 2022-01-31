import sys
import csv


def main():
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)
    else:
        # csv file
        with open(sys.argv[1], "r") as database:
            databaseDict = csv.DictReader(database)
            # convert to a list of dictionaries
            databaseDict = list(databaseDict)

            # text file
            with open(sys.argv[2], "r") as sequence:
                sequenceLine = sequence.readline().strip()

            # keys of databaseDict
            keys = list(databaseDict[0].keys())
            keys = keys[1:len(keys)]
            sequenceDict = {key: 0 for key in keys}

            # check how often the keys repeats
            for key in keys:
                amount = substring(sequenceLine, key)
                sequenceDict[key] = amount
                #print(key + ": " + str(amount))

            # check which person matches the sequence
            for row in databaseDict:
                matchingSequences = 0
                for key in keys:
                    if int(row[key]) == int(sequenceDict[key]):
                        matchingSequences = matchingSequences + 1

                if matchingSequences == len(keys):
                    print(row["name"])
                    sys.exit(0)

            print("No match")
            sys.exit(1)


def substring(sequenceLine, key):
    count = 0
    maxLen = round(len(sequenceLine) / len(key))

    for i in range(maxLen):
        if i * key in sequenceLine:
            count = i

    return count


if __name__ == "__main__":
    main()