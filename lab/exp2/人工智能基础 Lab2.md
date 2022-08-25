## 人工智能基础 Lab2

PB19111675  德斯别尔

### 传统机器学习

---

#### 决策树

决策树生成算法：

![image-20220628205540360](C:\Users\Deisbeir\AppData\Roaming\Typora\typora-user-images\image-20220628205540360.png)

其中给定了类`DecisionTree`以及其中的两个接口：

* `fit(train_features, train_labels)`,在这个函数中根据训练数据生成一棵决策树。
* `predict(test_features)`,在这个函数中你需要根据已经生成的决策树来预测标签。

选择最优属性划分以及节点生成：

```python
def TreeGenerate(self,train_features,train_labels,A):
        node = Treenode()
        if len(A) == 0 or len(set(train_labels)) <= 1 or len(train_features) <= 1:   # label均相同，或无可划分的属性集
            node.label = np.argmax(np.bincount(train_labels))   # 返回 train_labels 的众数
            return node
        # 选择期望剩余熵最小的作为最优划分
        av = A[np.argmin([Remainder(i,train_features,train_labels) for i in A])]
        # 属性a的值域
        range_av = set([i[av] for i in train_features])
        
        for l in range_av:
            Dvfeatures = [ feat for feat in train_features if feat[av] == l] 
            Dvlabels   = [ train_labels[i] for i in range(len(train_labels)) if train_features[i][av] == l]
            # Dv不会是空集
            node.child.append([ l, self.TreeGenerate(Dvfeatures,Dvlabels,[a for a in A if a != av])])
        
        node.leaf = False # 改为不是叶子
        node.attr = av      # 该节点是依据attr划分的
        return node
```

**运行结果**：

![image-20220628204011973](C:\Users\Deisbeir\AppData\Roaming\Typora\typora-user-images\image-20220628204011973.png)



#### 支持向量机

实验要求使用软间隔SVM来完成实验。

![image-20220628211507355](C:\Users\Deisbeir\AppData\Roaming\Typora\typora-user-images\image-20220628211507355.png)
$$
min_w \frac {1}{2} + C \sum_{N=1}^N |[y_n \neq sign(w^Tz_n+b)]|
$$

$$
s.t. y_n(w^Tx_n+b) \geq 1- \xi_i ,  \xi_i>0
$$



**二次型**

```python
 # 二次型规划
        P = np.array([[train_labels[i]*train_labels[j]*self.KERNEL(train_features[i],train_features[j]) for j in range(m)] for i in range(m)])
        q = np.array([-1]*m)
        G = np.array(list(-1*np.eye(m)) + list(np.eye(m)))
        h = np.array([0]*m + [self.C]*m) 
        A = np.array([train_labels])
        b = np.array([0])
        # cvxopt求解二次型规划
        Pc = matrix(P,tc='d')
        qc = matrix(q,tc='d')
        Gc = matrix(G,tc='d')
        hc = matrix(h,tc='d')
        Ac = matrix(A,tc='d')
        bc = matrix(b,tc='d')
        sol = solvers.qp(Pc,qc,Gc,hc,Ac,bc)
        a = sol['x']
```



**运行结果**：

![image-20220628204011973](C:\Users\Deisbeir\AppData\Roaming\Typora\typora-user-images\image-20220628204011973.png)

### 深度学习

---

#### 感知机模型

**感知机模型**：

![image-20220628213821696](C:\Users\Deisbeir\AppData\Roaming\Typora\typora-user-images\image-20220628213821696.png)

**前向传播**:

![image-20220628214739413](C:\Users\Deisbeir\AppData\Roaming\Typora\typora-user-images\image-20220628214739413.png)

**反向传播**：

![image-20220628214105879](C:\Users\Deisbeir\AppData\Roaming\Typora\typora-user-images\image-20220628214105879.png)

**运行结果**：

![image-20220628201224771](C:\Users\Deisbeir\AppData\Roaming\Typora\typora-user-images\image-20220628201224771.png)

#### 卷积神经网络

对卷积神经网络的初步掌握，实现图像分类。

![image-20220628220512459](C:\Users\Deisbeir\AppData\Roaming\Typora\typora-user-images\image-20220628220512459.png)

学号为 PB19111675 ，最后两位相加然后模6再加1计算得出自己的模型。
$$
(7+5)\%6+1=1
$$
既选择列表中第一个模型。

| 编号 | layer1 | layer2   | layer3 | layer4   | layer5 | layer6 | layer7 | layer8 | 激活函数 |
| ---- | ------ | -------- | ------ | -------- | ------ | ------ | ------ | ------ | -------- |
| --   | 2d卷积 | 池化     | 2d卷积 | 池化     | 卷积   | 线性层 | 线性层 | 线性层 | --       |
| 1    | 16，5  | 最大池化 | 32，5  | 最大池化 | -      | 120    | 84     | 10     | tanh     |

表格说明：

* 2d卷积（a,b) a:kernel个数，b:kernel size为（b,b)
* 默认池化大小为2
* 线性层 b:output channel的大小 (32 * 5 * 5 )
* 激活函数 在每个卷积和线性层后都加入激活函数，池化层无需添加
* -表示没有



**运行结果**：

![image-20220628203158297](C:\Users\Deisbeir\AppData\Roaming\Typora\typora-user-images\image-20220628203158297.png)