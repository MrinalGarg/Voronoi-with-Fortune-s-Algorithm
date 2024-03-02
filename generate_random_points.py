import sys
import random

def generate_random_points(num_points):
    points = []
    points.append(str(num_points))  # Add the number of points as the first line
    for _ in range(num_points):
        x = random.uniform(0, 10)  # Adjust range as needed
        y = random.uniform(0, 10)  # Adjust range as needed
        points.append(f"{x} {y}")
    return points

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python generate_random_points.py <num_points>")
        sys.exit(1)

    num_points = int(sys.argv[1])

    points = generate_random_points(num_points)
    for point in points:
        print(point)
