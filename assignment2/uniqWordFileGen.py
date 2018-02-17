import sys
import random

# This script will open a file in argv[1] and number of unique words in argv[2]
# and generate a text file with that many uniq words with argv[3] word counts.
# The delimiter for words is an optional argv[4]

def incStr(string):
    if string is "":
        return 'a'
    elif string[-1] >= 'z':
        return incStr(string[:-1]) + 'a'
    else:
        return string[:-1] + chr(ord(string[-1]) + 1)

def main():
    output = sys.argv[1]
    numUniq = int(sys.argv[2])
    wordCountEach = int(sys.argv[3])
    if len(sys.argv) < 5:
        delimiter = "!@#$%^&*()-_ =+,<.>/?[{]}|;:'" 
    else:
        delimiter = sys.argv[4]
    dictionary = {}
    string = ""

    while numUniq > 0:
        string = incStr(string)
        dictionary[string] = wordCountEach
        numUniq -= 1

    with open(output, "w+") as f:
        for string in dictionary.keys():
            while dictionary[string] > 0:
                f.write(string + delimiter)
                dictionary[string] -= 1

    return dictionary

main()
