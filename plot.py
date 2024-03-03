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

    # Get the minimum and maximum coordinates of input points
    min_x_input = min(point[0] for point in points)
    max_x_input = max(point[0] for point in points)
    min_y_input = min(point[1] for point in points)
    max_y_input = max(point[1] for point in points)

    # Get the minimum and maximum coordinates of edge points
    min_x_edges = min(min(edge[0][0], edge[1][0]) for edge in edges)
    max_x_edges = max(max(edge[0][0], edge[1][0]) for edge in edges)
    min_y_edges = min(min(edge[0][1], edge[1][1]) for edge in edges)
    max_y_edges = max(max(edge[0][1], edge[1][1]) for edge in edges)

    # Calculate the center of the input points
    center_x_input = (min_x_input + max_x_input) / 2
    center_y_input = (min_y_input + max_y_input) / 2

    # Calculate the center of the edge points
    center_x_edges = (min_x_edges + max_x_edges) / 2
    center_y_edges = (min_y_edges + max_y_edges) / 2

    # Calculate the width and height of the input and edge points
    width_input = max_x_input - min_x_input
    height_input = max_y_input - min_y_input
    width_edges = max_x_edges - min_x_edges
    height_edges = max_y_edges - min_y_edges

    # Determine the maximum width and height among input and edge points
    max_width = max(width_input, width_edges)
    max_height = max(height_input, height_edges)

    # Determine the padding for zooming out
    padding_factor = 0.1
    padding_width = padding_factor * max_width
    padding_height = padding_factor * max_height

    # Set the limits of the plot centered on the input points
    plt.xlim(center_x_input - max_width / 2 - padding_width, center_x_input + max_width / 2 + padding_width)
    plt.ylim(center_y_input - max_height / 2 - padding_height, center_y_input + max_height / 2 + padding_height)

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