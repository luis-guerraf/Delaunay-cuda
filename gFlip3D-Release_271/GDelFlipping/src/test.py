import numpy as np
import torch
import Delaunay_module      # Import after numpy and torch

vec = torch.FloatTensor(100, 3).uniform_(-10, 10)
print(vec)
output = Delaunay_module.triangulation(vec.numpy())
print(output)
