from itertools import izip


def parse(input_file, output_file):
    in_file = open(input_file, 'r')
    lines = map(lambda o: o.strip().split(','), in_file.readlines())

    groups = [{} for o in xrange(len(lines[0]))]

    for line in lines:
        filter(lambda o: o[0].update({o[1]: o[0].get(o[1], len(o[0]))}),
               izip(groups, line))

    with open(output_file, 'w') as out_file:
        for line in lines:
            out_file.write('%d' % groups[-1][line[-1]])
            for position, value in enumerate(izip(groups, line[:-1]), start=1):
                val = 1. / (len(value[0]) - 1) * value[0][value[1]]
                out_file.write(' %d:%f' % (position, val))
            out_file.write('\n')


if __name__ == '__main__':
    import sys
    input_file = sys.argv[1]
    if len(sys.argv) == 2:
        output_file = 'output.dat'
    else:
        output_file = sys.argv[2]
    parse(input_file, output_file)
