import 'package:flutter/material.dart';
import 'controller/controller.dart';
import 'components/speedSelector.dart';
import 'components/modeSelector.dart';
import 'components/controlAxis.dart';
import 'components/controlCoupole.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Jonckheere',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: MyHomePage(),
    );
  }
}


class MyHomePage extends StatefulWidget {
  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  bool hasError = false;
  late Controller controller;



  void stateChanged(bool error) {
    setState(() {
      hasError=error;
    });
  }

  @override
  void initState() {
    super.initState();

    controller = Controller(stateChanged);

  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Jonckheere mount control'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            
            SizedBox(height: 20),
            Container(
                width: 30,
                height: 30,
                decoration: BoxDecoration(
                  shape: BoxShape.circle,
                  color: hasError ? Colors.red : Colors.green, // Rouge si problème, Vert sinon
                ),
              ),
              SizedBox(height: 20),
              ModeSelector(modeSelected: controller.modeSelections, onValueChanged: controller.changeMode),
              SizedBox(height: 20),
              SpeedSelector(speedSelections : controller.speedSelections, onValueChanged: controller.changeSpeed),
              ControlAxis(control:controller),
              ControlCoupole(control:controller),
          ],
        ),
      ),
    );
  }
}
