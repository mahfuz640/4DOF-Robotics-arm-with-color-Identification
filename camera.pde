import processing.video.*;
import processing.serial.*;

Capture cam;
Serial myPort;

color detectedColor;
String lastSent = "";
int tolerance = 30;  // Change this to fine-tune sensitivity

void setup() {
  size(640, 480);
  
  String[] cameras = Capture.list();
  if (cameras.length == 0) {
    println("No cameras available!");
    exit();
  }

  cam = new Capture(this, cameras[0]);
  cam.start();

  String portName = Serial.list()[0];  // Adjust port index as needed
  myPort = new Serial(this, portName, 9600);
}

void draw() {
  if (cam.available()) {
    cam.read();
    image(cam, 0, 0);

    // Draw detection area
    int cx = width / 2;
    int cy = height / 2;
    int size = 10;
    stroke(255, 0, 0);
    noFill();
    rect(cx - size/2, cy - size/2, size, size);

    color c = cam.get(cx, cy);
    fill(c);
    noStroke();
    rect(10, 10, 100, 30);  // Display detected color

    String colorCode = detectColor(c);
    if (colorCode != null && !colorCode.equals(lastSent)) {
      myPort.write(colorCode);  // Send 'R', 'G', 'B', or 'U'
      println("Sent to Arduino: " + colorCode);
      lastSent = colorCode;
      delay(2000);  // Delay to prevent rapid spamming
    }
  }
}

String detectColor(color c) {
  float r = red(c);
  float g = green(c);
  float b = blue(c);

  if (abs(r - g) < tolerance && abs(g - b) < tolerance) {
    return "U";  // Gray/undefined
  } else if (r > g + tolerance && r > b + tolerance) {
    return "R";
  } else if (g > r + tolerance && g > b + tolerance) {
    return "G";
  } else if (b > r + tolerance && b > g + tolerance) {
    return "B";
  } else {
    return "U";  // If nothing dominant
  }
}
