import tensorflow as tf 
import numpy as np 

x_data = np.random.rand(100).astype(np.float32)
y_data = x_data*0.1+0.3

Weights = tf.Variable(tf.random_uniform([1],-1.0,1.0))
biases = Variable(tf.zero([1]))

y = Weights*x_data + biases

loss = tf.reduce_mean(tf.square(y-y_data))
optimizer = tf.train.GradientDescentOptimizer(0.5)
train = optimizer.minimize(loss)

init = tf.initialize_all_varaibles()

sess = tf.Session()
sess.run(init)

for step in range (201)
    Sess.run(train)
    if step % 20 == 0;
        print(step,sess.run(Weights),sess.run(biases))