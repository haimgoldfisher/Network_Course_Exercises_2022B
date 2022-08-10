import matplotlib.pyplot as plt
import numpy as np

reno = [0.01528,	 0.443811,	 1.290862,	 6.70851,	 6.70851,	6.70851]
cubic = [0.01181,	 0.472621,	 1.829061,	 1.343359,	 16.582232,	 25]
label = ["0%",      "10%",     "15%",      "20%",      "25%",       "30%"]
plt.plot(label, reno)
plt.plot(label, cubic)
plt.legend(["reno", "cubic"])
plt.show()