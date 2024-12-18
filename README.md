![XR2ino Application](https://github.com/TheMetagate/XR2ino_Tutorial/blob/main/Assets/Images/HeroCover_XR2ino.png)

# XR2ino

Metagate is an Italian startup developing Mixed Reality experiences for the Meta Quest. Our team creates customized applications for clients and original experiences released directly by the company. In this case, we wanted to experiment with using sensors and Arduino to make Mixed Reality even more engaging by connecting it to the physical world, embracing the potential of Extended Reality.

This code is designed to connect an Arduino BLE device to the XR2ino app, developed in Unity, for seamless integration with Mixed Reality experiences on Meta Quest headsets.

# Meta Quest Store

You can find the XR2ino app on the Meta Quest Store:
[**Download XR2ino from the Meta Store**](https://discover.themetagate.it/en/pages/xr2ino)


# Arduino Script

Our goal was to establish communication between Arduino and Unity via a Bluetooth module and ensure that the application could be customized by the end user simply by modifying the provided Arduino template.
In this way, we wanted to provide the Arduino community the opportunity to experiment with Mixed Reality without needing to work on Unity.

!! Be careful to follow our comments in the Arduino script. [**Download XR2ino code here**](https://github.com/TheMetagate/XR2ino_Tutorial/blob/main/Assets/Arduino/Script/XR2inoTemplate.ino)


### Variables

The Arduino code was designed to handle up to 10 variables of type 'float,' each with a customizable string variable associated with it to indicate the title or description of the float value. This title will appear on the dedicated panel within the app.
Unity does not allow special characters such as '%', '&', '°', etc. The application will not function as expected in such cases.

![XR2ino Application](https://github.com/TheMetagate/XR2ino_Tutorial/blob/main/Assets/Images/ScreenShotXR2ino.png)


### Sensors

As a test, we used a 'Capacitive Soil Moisture Sensor,' but any sensor can be used as long as the final data obtained is converted into a float. Over time, based on feedback received, we will add a list of sensors tested for the application.
About the Bluetooth connection, we used an Arduino Uno R4 WiFi, but any other Bluetooth Low Energy (BLE) module are allowed.


# License

This project is licensed under the [MIT LICENSE](https://github.com/TheMetagate/XR2ino_Tutorial/blob/main/LICENSE.txt).


# Support


