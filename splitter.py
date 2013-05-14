import random


def split_file(input_file, train_file, test_file):
    in_file = open(input_file, 'r')
    train = open(train_file, 'w')
    test = open(test_file, 'w')
    [train.write(line) if random.randint(0, 3)
        else test.write(line) for line in in_file]
    test.close()
    train.close()
    in_file.close()


if __name__ == '__main__':
    import sys
    in_file = sys.argv[1]
    if len(sys.argv) == 2:
        train_file = 'train.dat'
        test_file = 'test.dat'
    elif len(sys.argv) == 3:
        train_file = sys.argv[2]
        test_file = 'test.dat'
    else:
        train_file = sys.argv[2]
        test_file = sys.argv[3]

    split_file(in_file, train_file, test_file)
