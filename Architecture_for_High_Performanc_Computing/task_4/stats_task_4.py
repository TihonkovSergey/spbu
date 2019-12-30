from prettytable import PrettyTable
import matplotlib as plt

# BLOCKS
f_blocks = open('output_blocks_prod.txt', "r")
lines_blocks = f_blocks.readlines()

dim_list = []
for dim in lines_blocks[0].split(" ")[:-1]:
    dim_list.append(dim)

t_blocks = PrettyTable(['num of threads \ dimension'] + dim_list)

for line in lines_blocks[1::]:
    t_blocks.add_row(line.split())
f_blocks.close()



# TAPES
f_tapes = open('output_tapes_prod.txt', "r")
lines_tapes = f_tapes.readlines()


t_tapes = PrettyTable(['num of threads \ dimension'] + dim_list)

for line in lines_tapes[1::]:
    t_tapes.add_row(line.split())
f_tapes.close()

# OUTPUT TABLES
print("Разделение данных по блокам:")
print(t_blocks)
print()
print("Ленточное разделение данных:")
print(t_tapes)