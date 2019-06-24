# Reco Facial

## compilation du projet

```compile
make
```

ou

```compile
g++ -g -Wall -o face faceRecognition.cpp `pkg-config opencv --cflags --libs`
```

## Execute training

```compile
g++ -g -Wall -o train training.cpp `pkg-config opencv --cflags --libs`
```

```run
./train [Path to csv mapping]
```

This file must be like to following example:

```csv
path/[name].pgm;ID
path/[name].pgm;ID
```

## Execute face recognition

```run
./face [Path to the Haar Cascade for face detection]
```

A training file must be provided. His name: "trainer.yml"

## Version Opencv

 Opencv 3.2.0

## Image format

The format used for recognition is 92 by 92

## Add custom image

```compile
g++ -g -Wall -o take takePhoto.cpp `pkg-config opencv --cflags --libs`
```

```run
./take
```
