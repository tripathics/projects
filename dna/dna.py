# Check for name of person given their DNA sequence
import sys
import csv


def main():

    # Check proper usage
    if not len(sys.argv) == 3:
        print("Usage: python dna.py data.csv sequence.txt")
    
    # loading input files into memory
    database = []
    with open(sys.argv[1], "r") as data:
        reader = csv.DictReader(data)
        for person in reader:
            for key in person:
                if not key == 'name':
                    person[key] = int(person[key])
            database.append(person)

    # list of Short-Tandem-Repeats in the database
    STRs = []
    for key in database[0]:
        if not key == 'name':
            STRs.append(key)

    with open(sys.argv[2]) as seq:
        for line in seq:
            sequence = line.rstrip()

    # printing the result
    seq_str_cnt = count_strs(sequence, STRs)
    
    for person in database:
        n_matches = 0
        for STR in seq_str_cnt:
            if seq_str_cnt[STR] == person[STR]:
                n_matches += 1
        if n_matches == len(STRs):
            print(person["name"])
            return 0
    
    print("No match")
    return 0


# Count the number of STRs in the given sequence of DNA

def count_strs(seq, STRs):
    STRs_count = {}
    curr_str_count = 0
    # Iterating over all the STRs in the database
    for STR in STRs:
        # Iterating over each character in the loop
        i = 0
        while i < len(seq):
            # checking if STR matched
            if seq[i:i+len(STR)] == STR:
                curr_str_count += 1
                i += len(STR) - 1

            # updating the STRs_count dict with number of STRs calculated  
            elif curr_str_count > 0:
                if not STR in STRs_count or STRs_count[STR] < curr_str_count:
                    STRs_count[STR] = curr_str_count
                curr_str_count = 0
            i += 1
    
    return STRs_count


if __name__ == '__main__':
    main()