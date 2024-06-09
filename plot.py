import matplotlib.pyplot as plt
import pandas as pd
import sys

# Load the CSV file
data = pd.read_csv(sys.argv[1])

data = data[data["time"] > 0.5 * 1e9]

fps = data.dropna(subset=["fps"])
updates = data.dropna(subset=["fixed"])


# Plotting the data
plt.figure(figsize=(10, 6))
# plt.scatter(updates["time"], updates["fixed"], label="Updates", color="r")
plt.plot(fps["time"], fps["fps"], label="FPS over Time", color="b")


avg = sum(fps["fps"]) / len(fps["fps"])
plt.axhline(
    y=avg,
    color="g",
    linestyle="--",
    label=f"avg: {round(avg)} fps",
)
m = min(fps["fps"])
plt.axhline(
    y=m,
    color="g",
    linestyle=":",
    label=f"min: {round(m)} fps",
)

plt.axhline(
    y=60,
    color="r",
    linestyle=":",
)

# Adding labels and title
plt.xlabel("Time")
plt.ylabel("FPS")
plt.title("FPS Over Time")
plt.legend()

# Display the plot
plt.grid(True)
plt.show()
