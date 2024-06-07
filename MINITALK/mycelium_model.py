import tensorflow as tf
from tensorflow.keras.layers import Dense, Dropout, ReLU, Layer
from tensorflow.keras.models import Model
from tensorflow.keras.callbacks import Callback
import numpy as np

class MyceliumLayer(Layer):
    def __init__(self, units, **kwargs):
        super(MyceliumLayer, self).__init__(**kwargs)
        self.units = units

    def build(self, input_shape):
        self.kernel = self.add_weight(name='kernel',
                                      shape=(input_shape[-1], self.units),
                                      initializer='glorot_uniform',
                                      trainable=True)
        self.bias = self.add_weight(name='bias',
                                    shape=(self.units,),
                                    initializer='zeros',
                                    trainable=True)
        super(MyceliumLayer, self).build(input_shape)

    def call(self, inputs, training=None):
        sigmoid_inputs = tf.nn.sigmoid(inputs)
        dynamic_kernel = self.kernel * tf.expand_dims(sigmoid_inputs, -1)
        dynamic_kernel = tf.reduce_sum(dynamic_kernel, axis=1)
        output = tf.matmul(inputs, self.kernel) + self.bias  # Corrected dynamic_kernel to self.kernel
        return output

    def get_config(self):
        config = super(MyceliumLayer, self).get_config()
        config.update({"units": self.units})
        return config

class DynamicMyceliumModel(Model):
    def __init__(self, input_shape, num_classes):
        super(DynamicMyceliumModel, self).__init__()
        self.mycelium_layers = [MyceliumLayer(128)]
        self.relu_layers = [ReLU()]
        self.dropout_layers = [Dropout(0.5)]
        self.output_layer = Dense(num_classes, activation='softmax')
        self.build((None, *input_shape))

    def call(self, inputs, training=None):
        x = inputs
        for layer, relu, dropout in zip(self.mycelium_layers, self.relu_layers, self.dropout_layers):
            x = layer(x, training=training)
            x = relu(x)
            x = dropout(x, training=training)
        x = self.output_layer(x)
        return x

    def add_layer(self):
        new_mycelium_layer = MyceliumLayer(64)
        new_relu_layer = ReLU()
        new_dropout_layer = Dropout(0.5)
        self.mycelium_layers.append(new_mycelium_layer)
        self.relu_layers.append(new_relu_layer)
        self.dropout_layers.append(new_dropout_layer)
        # Rebuild model with new layers
        self.build((None, 784))

class DynamicCallback(Callback):
    def __init__(self, dynamic_model, patience=2):
        super(DynamicCallback, self).__init__()
        self.dynamic_model = dynamic_model
        self.patience = patience
        self.best_val_loss = np.Inf
        self.wait = 0

    def on_epoch_end(self, epoch, logs=None):
        current_val_loss = logs.get("val_loss")
        if current_val_loss < self.best_val_loss:
            self.best_val_loss = current_val_loss
            self.wait = 0
        else:
            self.wait += 1
            if self.wait >= self.patience:
                # Save weights
                weights = self.dynamic_model.get_weights()
                # Add new layer
                self.dynamic_model.add_layer()
                # Recompile the model
                self.dynamic_model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
                # Restore weights
                self.dynamic_model.set_weights(weights)
                self.wait = 0

# Define model
input_shape = (784,)  # Example for MNIST data
num_classes = 10

dynamic_model = DynamicMyceliumModel(input_shape, num_classes)
dynamic_model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])

# Print model summary
dynamic_model.build((None, 784))  # Build the model with dynamic layers
dynamic_model.summary()

# Load and preprocess data
(x_train, y_train), (x_test, y_test) = tf.keras.datasets.mnist.load_data()
x_train = x_train.reshape(-1, 784).astype('float32') / 255
x_test = x_test.reshape(-1, 784).astype('float32') / 255

# Train model with dynamic callback
callback = DynamicCallback(dynamic_model)
dynamic_model.fit(x_train, y_train, epochs=10, batch_size=32, validation_data=(x_test, y_test), callbacks=[callback])

# Evaluate model on test data
test_loss, test_acc = dynamic_model.evaluate(x_test, y_test)
print(f"Test accuracy: {test_acc}")

# Make predictions
predictions = dynamic_model.predict(x_test)
print(predictions[0])

















# Wyjaśnienie Kodu

#     MyceliumLayer:
#         Warstwa inspirowana grzybnią, która dynamicznie modyfikuje swoje wagi.
#         Zmiany wag są realizowane przez mnożenie wag przez sigmoid funkcję wejściowych, co symuluje dynamiczną adaptację.

#     build_mycelium_model:
#         Tworzy model sieci neuronowej z użyciem warstw MyceliumLayer.
#         Dwie warstwy MyceliumLayer są używane, a następnie klasyczna warstwa Dense do klasyfikacji.

#     Trening Modelu:
#         Model jest trenowany na danych MNIST.
#         Dane są przekształcane i normalizowane, a następnie model jest trenowany przez 10 epok.

# Podsumowanie

# Ten koncepcyjny model demonstruje, jak można zaimplementować sieć neuronową inspirowaną grzybnią. Kluczowym elementem jest MyceliumLayer, która dynamicznie dostosowuje swoje połączenia, co naśladuje sposób działania grzybni. Ten podejście może być rozwijane i modyfikowane w celu lepszego odwzorowania biologicznych mechanizmów adaptacji i komunikacji grzybni.


# Aby przetestować kod, musisz mieć zainstalowane niezbędne biblioteki i dane. Poniżej znajdziesz kroki, jak przetestować ten kod na swoim komputerze.
# Krok 1: Instalacja Wymaganych Bibliotek

# Upewnij się, że masz zainstalowane następujące biblioteki: TensorFlow i NumPy. Możesz je zainstalować za pomocą pip.

# sh

# pip install tensorflow numpy

# Krok 2: Uruchomienie Kodu

# Stwórz nowy plik Python, na przykład mycelium_model.py, i skopiuj poniższy kod do tego pliku:

# python

# import tensorflow as tf
# from tensorflow.keras.layers import Dense, Input, Layer
# from tensorflow.keras.models import Model

# # Definicja adaptacyjnej warstwy inspirowanej grzybnią
# class MyceliumLayer(Layer):
#     def __init__(self, units, **kwargs):
#         super(MyceliumLayer, self).__init__(**kwargs)
#         self.units = units

#     def build(self, input_shape):
#         self.kernel = self.add_weight(name='kernel',
#                                       shape=(input_shape[-1], self.units),
#                                       initializer='glorot_uniform',
#                                       trainable=True)
#         self.bias = self.add_weight(name='bias',
#                                     shape=(self.units,),
#                                     initializer='zeros',
#                                     trainable=True)
#         super(MyceliumLayer, self).build(input_shape)

#     def call(self, inputs, training=None):
#         # Dynamically modify weights to simulate adaptation (e.g., based on input activity)
#         dynamic_kernel = self.kernel * tf.nn.sigmoid(inputs)
#         output = tf.matmul(inputs, dynamic_kernel) + self.bias
#         return output

#     def get_config(self):
#         config = super(MyceliumLayer, self).get_config()
#         config.update({"units": self.units})
#         return config

# # Budowa modelu inspirowanego grzybnią
# def build_mycelium_model(input_shape, num_classes):
#     inputs = Input(shape=input_shape)
#     x = MyceliumLayer(128)(inputs)
#     x = tf.keras.layers.ReLU()(x)
#     x = MyceliumLayer(64)(x)
#     x = tf.keras.layers.ReLU()(x)
#     outputs = Dense(num_classes, activation='softmax')(x)

#     model = Model(inputs=inputs, outputs=outputs)
#     return model

# # Definicja modelu
# input_shape = (784,)  # Przykład dla danych MNIST
# num_classes = 10

# model = build_mycelium_model(input_shape, num_classes)
# model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
# model.summary()

# # Trening modelu
# (x_train, y_train), (x_test, y_test) = tf.keras.datasets.mnist.load_data()
# x_train = x_train.reshape(-1, 784).astype('float32') / 255
# x_test = x_test.reshape(-1, 784).astype('float32') / 255

# model.fit(x_train, y_train, epochs=10, batch_size=32, validation_data=(x_test, y_test))

# Krok 3: Uruchomienie Skryptu

# Otwórz terminal, przejdź do katalogu, w którym zapisałeś mycelium_model.py, i uruchom skrypt:

# sh

# python mycelium_model.py

# Co Powinno się Wydarzyć

#     Kompilacja Modelu: Kod powinien wyświetlić podsumowanie modelu, pokazując warstwy i ich kształty.
#     Trening Modelu: Model zacznie trening na zbiorze danych MNIST. Powinieneś zobaczyć wyniki treningu wyświetlane dla każdej epoki, w tym stratę (loss) i dokładność (accuracy).
#     Walidacja Modelu: Po każdej epoce, model będzie oceniany na zbiorze walidacyjnym, co pozwoli monitorować jego wydajność.

# Testowanie Kodów

# Jeśli chcesz dodać testy, aby upewnić się, że poszczególne części działają poprawnie, możesz napisać kilka prostych testów z użyciem biblioteki unittest lub pytest. Na przykład:

# python

# import unittest

# class MyceliumLayerTest(unittest.TestCase):
#     def test_layer_output_shape(self):
#         layer = MyceliumLayer(128)
#         input_tensor = tf.random.normal([1, 784])
#         output_tensor = layer(input_tensor)
#         self.assertEqual(output_tensor.shape, (1, 128))

# class MyceliumModelTest(unittest.TestCase):
#     def test_model_output_shape(self):
#         model = build_mycelium_model((784,), 10)
#         input_tensor = tf.random.normal([1, 784])
#         output_tensor = model(input_tensor)
#         self.assertEqual(output_tensor.shape, (1, 10))

# if __name__ == '__main__':
#     unittest.main()

# Uruchomienie tych testów:

# sh

# python -m unittest mycelium_model.py

# To podejście pozwoli Ci upewnić się, że poszczególne komponenty działają zgodnie z oczekiwaniami, zanim przejdziesz do pełnego treningu modelu.