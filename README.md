# Optical Sensor Toolkit

[![Join the chat at https://gitter.im/Abbath/P](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/Abbath/P?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

This is my old master's degree project. It's not even an alpha :-). It includes modeling and measurement tool for optical pressure sensor

Features
--
* Modeling of membrane deflection and stress using Kirchoff-Love thin plate theory 
* Modeling of laser beam relfection on the surface of the membrane
* Measurement of the pressure based on least squares interpolation using previously prepared samples and image detection using OpenCV
* SQLITE databes with prepared calibration samples and sensor parameter sets 

TODO's
--
* Improve light reflection model on the holes array in membrane 2 (Not just "Sweet Jesus, I see a hole, let me go through it") 
* Use some FEM engine like GetFEM++ to model deflection of the membrane 1 (Kirchoff-Love model is lame for these purposes and I had to create unicorns and magic)
* Improve import system
* Make documentation & manual
* Eliminate unicorns
