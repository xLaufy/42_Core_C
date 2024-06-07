import numpy as np
import tensorflow as tf
from tensorflow.keras.layers import Dense, Input
from tensorflow.keras.models import Model

# Definiowanie adaptacyjnych połączeń, które mogą się zmieniać w trakcie uczenia
class MyceliumLayer(tf.keras.layers.Layer):
    def __init__(self, units):
        super(MyceliumLayer, self).__init__()
        self.units = units

    def build(self, input_shape):
        self.kernel = self.add_weight(name='kernel',
                                      shape=(input_shape[-1], self.units),
                                      initializer='uniform',
                                      trainable=True)

    def call(self, inputs):
        # Możliwość dynamicznej zmiany połączeń
        connections = tf.nn.sigmoid(self.kernel)
        outputs = tf.matmul(inputs, connections)
        return outputs

# Definiowanie modelu inspirowanego grzybnią
input_layer = Input(shape=(784,))
x = MyceliumLayer(128)(input_layer)
x = Dense(64, activation='relu')(x)
output_layer = Dense(10, activation='softmax')(x)

model = Model(inputs=input_layer, outputs=output_layer)
model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
model.summary()
