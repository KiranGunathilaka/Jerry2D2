import xlwings as xw

# Open the Excel workbook
wb = xw.Book('plotter.xlsx')

# Select the sheet you want to read from
sheet = wb.sheets['Data In']  # Replace 'Sheet1' with the name of your sheet

# Generate an 11x11 matrix filled with '#'
maze = [['#' for _ in range(11)] for _ in range(11)]
cell = [0,0]
prev_cell = [-1,-1]
move = 0

while True:
    data_range = sheet.range('D5:G5').expand('table')
    data = data_range.value

    cell=[int(data[0,0]),int(data[0,1])]
    if cell!= prev_cell:
        maze[(10-2*cell[0,0]),(1+2*cell[0,1])] = "^"
        if data[0,2]== 1:
            maze[(10-2*cell[0,0]-1),(1+2*cell[0,1])] = " "
        if data[0,3]== 1:
            maze[(10-2*cell[0,0]),(1+2*cell[0,1]+1)] = " "
        if data[0,4]== 1:
            maze[(10-2*cell[0,0]+1),(1+2*cell[0,1])] = " "
        if data[0,5]== 1:
            maze[(10-2*cell[0,0]),(1+2*cell[0,1]-1)] = " "
        # Print the generated maze
        print(cell)
        for row in maze:
            print(' '.join(row))
        move += 1


# Close the workbook
wb.close()
