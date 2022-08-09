import time

import torch
import matplotlib
import numpy as np

matplotlib.use("Qt5Agg")
import matplotlib.animation as animation
import matplotlib.pyplot as plt

from aestream import UDPInput

## Example modified from: https://matplotlib.org/stable/tutorials/advanced/blitting.html

# Initialize our canvas
fig, ax = plt.subplots()
image = ax.imshow(torch.zeros(480, 640*2), cmap="gray", vmin=0, vmax=1)
plt.show(block=False)
plt.pause(0.1)
bg = fig.canvas.copy_from_bbox(fig.bbox)
ax.draw_artist(image)
fig.canvas.blit(fig.bbox)

# Start streaming from two UDP streams
with UDPInput((640, 480), "cpu", 4301) as stream1:
    with UDPInput((640, 480), "cpu", 4302) as stream2:
        while True:
            tensor1 = stream1.read()
            tensor2 = stream2.read()

            # Redraw figure
            fig.canvas.restore_region(bg)

            numpy1 = tensor1.T.numpy()
            numpy2 = tensor2.T.numpy()
            combined = np.concatenate((numpy1, numpy2), axis=1)

            image.set_data(combined)
            ax.draw_artist(image)
            fig.canvas.blit(fig.bbox)
            fig.canvas.flush_events()

            # Pause to only loop 10 times per second
            plt.pause(0.01)
