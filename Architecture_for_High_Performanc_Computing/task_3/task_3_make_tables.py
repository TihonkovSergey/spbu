from prettytable import PrettyTable
import matplotlib as plt

# ROWS
f_rows = open('output_rows.txt', "r")
lines_rows = f_rows.readlines()

dim_list = []
for dim in lines_rows[0].split(" ")[:-1]:
    dim_list.append(dim)

t_rows = PrettyTable(['num of threads \ dimension'] + dim_list)

for line in lines_rows[1::]:
    t_rows.add_row(line.split())
f_rows.close()

# COLUMNS
t_cols = PrettyTable(['num of threads \ dimension'] + dim_list)

f_cols = open('output_columns.txt', "r")
lines_cols = f_cols.readlines()

for line in lines_cols[1::]:
    t_cols.add_row(line.split())
f_cols.close()

#BLOCKS
t_blocks = PrettyTable(['num of threads \ dimension'] + dim_list)

f_blocks = open('output_blocks.txt', "r")
lines_blocks = f_blocks.readlines()

for line in lines_blocks[1::]:
    t_blocks.add_row(line.split())
f_blocks.close()

# OUTPUT TABLES
print("Разделение данных по строкам:")
print(t_rows)
print()
print("Разделение данных по столбцам:")
print(t_cols)
print()
print("Разделение данных по блокам:")
print(t_blocks)