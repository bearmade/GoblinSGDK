def generate_grid_patterns():
    with open("grid.md", "w") as f:
        f.write("# All 256 permutations of 3x3 grid with center always 'x'\n\n")
        
        # Generate all possible combinations (2^8 = 256 patterns)
        for i in range(256):
            # Convert number to 8-bit binary representation
            binary = format(i, '08b')
            
            # Map each bit to the 8 positions around the center
            grid = [
                [binary[0] == '1', binary[1] == '1', binary[2] == '1'],
                [binary[7] == '1', True, binary[3] == '1'],  # Center is always 'x'
                [binary[6] == '1', binary[5] == '1', binary[4] == '1']
            ]
            
            # Write pattern header
            f.write(f"# Pattern {i+1}\n")
            
            # Write the grid
            f.write("| " + " | ".join(["x" if grid[0][j] else "." for j in range(3)]) + " |\n")
            f.write("|---|---|---|\n")
            f.write("| " + " | ".join(["x" if grid[1][j] else "." for j in range(3)]) + " |\n")
            f.write("| " + " | ".join(["x" if grid[2][j] else "." for j in range(3)]) + " |\n")
            f.write("\n")

if __name__ == "__main__":
    generate_grid_patterns()
