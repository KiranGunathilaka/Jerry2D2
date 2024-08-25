import xlwings as xw

# Open the Excel workbook
wb = xw.Book('plotter(version 2).xlsx')

# Select the sheet you want to read from
sheet = wb.sheets['Data In']  # Replace 'Sheet1' with the name of your sheet

# Generate an 11x11 matrix filled with '#'
maze = [['#' for _ in range(11)] for _ in range(11)]
cell = [0, 0]
prev_cell = [-1, -1]
move = 0

while True:
    data_range = sheet.range('B5:G5').expand('table')
    data = data_range.value

    # Assuming data is a list of lists, extract the first (and only) row
    cell = [int(data[0]), int(data[1])]

    if cell != prev_cell:
        # Mark current position with a "^"
        maze[9 - 2 * cell[0]][1 + 2 * cell[1]] = "^"
        
        # Check walls (assuming these are 1 or 0 for presence or absence)
        if data[2] == 0:  # North
            maze[9 - 2 * cell[0] - 1][1 + 2 * cell[1]] = " "
        if data[3] == 1:  # East
            maze[9 - 2 * cell[0]][1 + 2 * cell[1] + 1] = " "
        if data[4] == 2:  # South
            maze[9 - 2 * cell[0] + 1][1 + 2 * cell[1]] = " "
        if data[5] == 3:  # West
            maze[9 - 2 * cell[0]][1 + 2 * cell[1] - 1] = " "
        
        # Print the generated maze
        print(cell)
        for row in maze:
            print(' '.join(row))
        
        # Update previous cell
        prev_cell = cell.copy()
        
        move += 1

    # Add a condition to break out of the loop if necessary
    if move > 100:  # Example condition to prevent an infinite loop
        break

# Close the workbook
wb.close()
