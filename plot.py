import matplotlib.pyplot as plt

def read_points(filename):
    points = []
    with open(filename, 'r') as file:
        num_points = int(file.readline())
        for _ in range(num_points):
            x, y = map(float, file.readline().split())
            points.append((x, y))
    return points

def read_edges(filename):
    edges = []
    with open(filename, 'r') as file:
        for line in file:
            x1, y1, x2, y2 = map(float, line.split())
            edges.append(((x1, y1), (x2, y2)))
    return edges

def plot_points(points):
    x = [point[0] for point in points]
    y = [point[1] for point in points]
    plt.scatter(x, y, color='blue')

def plot_edges(edges):
    for edge in edges:
        x_values = [edge[0][0], edge[1][0]]
        y_values = [edge[0][1], edge[1][1]]
        plt.plot(x_values, y_values, 'r-')

def plot(input_file, output_file):
    points = read_points(input_file)
    edges = read_edges(output_file)

    plot_points(points)
    plot_edges(edges)

    plt.xlabel('X-axis')
    plt.ylabel('Y-axis')
    plt.title('Input Points and Output Edges')

    plt.grid(True)
    plt.gca().set_aspect('equal', adjustable='box')
    plt.show()

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        print("Usage: python plot.py <input_file> <output_file>")
        sys.exit(1)
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    plot(input_file, output_file)

#give code such that it generates a data set of 