import torch
import numpy as np
from matplotlib import pyplot as plt
from numpy import tanh

class MLP:
    def __init__(self,activationFunction,activationFunctionDerivative,layerSize=[10,19,13,7,4]):
        # layer size = [10, 8, 8, 4]
        # 初始化所需参数  
        self.layerSize = layerSize
        self.W = [None] + [np.random.normal(0,0.3,(self.layerSize[i+1],self.layerSize[i])) for i in range(len(self.layerSize)-1) ]
        self.biss = [None] + [ np.array([[0]*m]).T  for m in self.layerSize[1:] ]
        self.s = activationFunction             # 激活函数          s()
        self.sf = activationFunctionDerivative  # 激活函数的导数    s'()
        self.h = [None]*len(self.layerSize)     # 输出    h = s(z)
        self.z = [None]*len(self.layerSize)     # 输入    z = Wx + b ，其中x为上一层输出
        self.dw , self.db= [None]*len(self.layerSize),  [None]*len(self.layerSize)  # 反向传播求得的W与biss的梯度
        self.Loss = 0
        def CrossEntropyLoss(x, y):
            return  [-np.log(x[i][0]) for i in range(len(y)) if y[i][0] == 1].pop()
        self.lossFunction = CrossEntropyLoss    # 损失函数
        def Softmax(x):
            x = np.exp(x)
            return x/np.sum(x)
        self.s_out = Softmax                    # 输出层的激活函数

    def forward(self, x):   
        # 前向传播
        for i in range(len(self.layerSize)):    # 第i层向第i+1层传播
            if i == 0:                          # 输入层
                self.h[i] = np.array([x]).T
            elif i == len(self.layerSize) - 1:  # 输出层
                self.z[i] = self.W[i].dot(self.h[i-1]) + self.biss[i]
                self.h[i] = self.s_out(self.z[i])
            else:
                self.z[i] = self.W[i].dot(self.h[i-1]) + self.biss[i]
                self.h[i] = self.s(self.z[i])
        return  self.h[i].T


    def backward(self, y):  # 自行确定参数表
        # 反向传播
        y = np.array([y]).T
        # self.Loss = 0.5 * np.sum([i**2 for i in (self.h[len(self.layerSize) - 1] - y) ] )# 反向传播, 误差值，采用 Loss = 0.5*||Δy||_2^2
        self.Loss = self.lossFunction(self.h[-1],y)
        for i in range(len(self.layerSize)-1,0,-1):  # 第i层向第i-1层反向传播，用δ表示 dLoss/dW
            if i == len(self.layerSize)-1:
                delta = self.h[i] - y
            else:
                delta = self.W[i+1].T.dot(delta)*self.sf(self.z[i])
            self.dw[i] = delta.dot(self.h[i-1].T)
            self.db[i] = delta
        
    def update(self,lr):
        # 更新weight与biss
        for i in range(1,len(self.layerSize)):
            self.W[i] = self.W[i] - lr*self.dw[i]
            self.biss[i] = self.biss[i] - lr*self.db[i]
            

def train(mlp: MLP, epochs, lr, inputs, labels):
        # mlp: 传入实例化的MLP模型
        # epochs: 训练轮数
        # lr: 学习率
        # inputs: 生成的随机数据
        # labels: 生成的one-hot标签
    Loss = []
    for epoch in range(epochs):
        loss = 0
        for i in range(len(inputs)):
            output = mlp.forward(inputs[i])
            mlp.backward(labels[i])
            mlp.update(lr)
            loss = loss + mlp.Loss
        Loss.append(loss)
    return Loss
    
    


if __name__ == '__main__':
    # 设置随机种子,保证结果的可复现性
    np.random.seed(1)

    # 生成数据
    inputs = np.random.randn(100, 10)
    # 生成one-hot标签
    labels = np.eye(4)[np.random.randint(0, 4, size=(1, 100))].reshape(100, 4)

    def dtanxdx(x):
        return 1-tanh(x)**2


    mlp = MLP(tanh,dtanxdx,[10, 10, 8, 8, 4])

    # 训练
    epochs = 1000
    lr = 0.015
    l = train(mlp, epochs, lr, inputs, labels)
    fig, ax = plt.subplots()
    ax.plot(np.array(l), color='blue')
    plt.show()
