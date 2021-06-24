import sys, os

in_file = "/etc/bashrc"

ips = ["9.135.113.138", "9.134.69.87", "9.134.80.50", "9.134.76.179", "9.134.125.154", "9.135.113.187"]

f_in = open(in_file, "r")
lines = f_in.readlines()
os.remove(in_file)

pattern = "no_proxy="
pattern_len = len(pattern)

f_out = open(in_file, "a")

for line in lines:
    if(len(line) > pattern_len) and line[:9] == pattern:
        line = line.strip('\n')
        for ip in ips:
            line += ',' + ip
        line += '\n'

    f_out.write(line)