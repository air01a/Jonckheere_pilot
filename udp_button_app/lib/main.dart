import 'package:flutter/material.dart';
import 'services/communicator.dart';

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
  // Adresse IP et port du serveur UDP
  final String serverIp = "10.113.211.167";//"192.168.1.1";  // Remplacez par l'adresse IP de votre serveur
  final int serverPort = 4000;
  List<String> debugMessages = [];
  bool udp=false;
  List<bool> _selections = [true, false, false]; // Le premier est sélectionné par défaut
  String _selectedOption = 'sidereal';
  List<bool> _speedSelections = [true, false, false,false]; // Le premier est sélectionné par défaut
  String _speedSelectedOption = 'x1';
  bool hasError = false;
  late Communicator communicator; 


  // Fonction pour ajouter des messages à l'interface de débogage
  void addDebugMessage(String message) {
    setState(() {
      debugMessages.add(message);
    });
  }

  void manageResult(String message) {
    // Simule une opération asynchrone (ex : requête réseau, lecture de fichier, etc.)
    //await sendTcpMessage("getParams", serverIp, serverPort);  // Attente de 2 secondes pour la simulation
    List<String> segments = message.split('_');
    // Lorsque l'opération asynchrone est terminée, mettre à jour les données
    if (segments[0]=="OK") {
        setState(() {
          hasError=false;
        },);
    // Mettre à jour l'interface utilisateur avec les nouvelles données
      if (segments.length==3) {
        setState(() {
          _selectedOption=segments[1];
          _speedSelectedOption=segments[2];
          switch(_selectedOption) {
            case "sidereal":
              _selections = [true, false, false];
              break;
            case "solar":
              _selections=[false,false,true];
              break;
            case "lunar":
              _selections=[false,true,false];
          }
          switch (_speedSelectedOption) {
            case "x1":
              _speedSelections=[true,false,false,false];
              break;
            case "x2":
              _speedSelections=[false,true,false,false];
              break;
            case "x4":
              _speedSelections=[false,false,true,false];
              break;
            case "x16":
              _speedSelections=[false,false,false,true];
              break;
          }
      });
      }
    }

    // Exécuter le code que vous voulez après l'exécution asynchrone
  }

  void preSendFunction() {
    setState(() {
      hasError=true;
    });
  }

  @override
  void initState() {
    super.initState();
    communicator = Communicator(preSendFunction: preSendFunction, postSendFunction: manageResult, debug:addDebugMessage);
    communicator.sendTcpMessage("getParams", serverIp, serverPort);
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
            

          Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          ToggleButtons(
            isSelected: _selections,
            onPressed: (int index) {
              setState(() {
                for (int i = 0; i < _selections.length; i++) {
                  _selections[i] = i == index;  // Seul un bouton peut être sélectionné
                }
                switch (index) {
                  case 0:
                    _selectedOption = 'sidereal';
                    break;
                  case 1:
                    _selectedOption = 'lunar';
                    break;
                  case 2:
                    _selectedOption = 'solar';
                    break;
                };
                communicator.sendTcpMessage(_selectedOption, serverIp, serverPort);
              });
            },
            children: <Widget>[
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 20),
                child: Text('Sidereal'),
              ),
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 20),
                child: Text('Lunar'),
              ),
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 20),
                child: Text('Solar'),
              ),
            ],
          )]),
           SizedBox(height: 20),
        Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          ToggleButtons(
            isSelected: _speedSelections,
            onPressed: (int index) {
              setState(() {
                for (int i = 0; i < _speedSelections.length; i++) {
                  _speedSelections[i] = i == index;  // Seul un bouton peut être sélectionné
                }
                switch (index) {
                  case 0:
                    _speedSelectedOption = 'x1';
                    break;
                  case 1:
                    _speedSelectedOption = 'x2';
                    break;
                  case 2:
                    _speedSelectedOption = 'x4';
                    break;
                  case 3:
                    _speedSelectedOption = 'x16';
                    break;
                };
                communicator.sendTcpMessage(_speedSelectedOption, serverIp, serverPort);
              });
            },
            children: <Widget>[
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 20),
                child: Text('x1'),
              ),
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 20),
                child: Text('x2'),
              ),
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 20),
                child: Text('x4'),
              ),
              Padding(
                padding: const EdgeInsets.symmetric(horizontal: 20),
                child: Text('x16'),
              ),
            ],
          )]),
          GestureDetector(
            onTapDown: (_) {
              communicator.sendUdpMessage("DEC+", serverIp, serverPort);
            },
            onTapUp: (_) {
              communicator.sendUdpMessage("DEC", serverIp, serverPort);
            },
            child: Container(
              margin: EdgeInsets.all(20),
              padding: EdgeInsets.symmetric(horizontal: 40, vertical: 20),
              color: Colors.blue,
              child: Text(
                'DEC+',
                style: TextStyle(color: Colors.white, fontSize: 18),
              ),
            ),
          ),

          Row(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [

             GestureDetector(
            onTapDown: (_) {
              communicator.sendUdpMessage("AD-", serverIp, serverPort);
            },
            onTapUp: (_) {
              communicator.sendUdpMessage("AD", serverIp, serverPort);
            },
            child: Container(
              margin: EdgeInsets.all(20),
              padding: EdgeInsets.symmetric(horizontal: 40, vertical: 20),
              color: Colors.blue,
              child: Text(
                'AD-',
                style: TextStyle(color: Colors.white, fontSize: 18),
              ),
            ),
          ),

           GestureDetector(
            onTapDown: (_) {
              communicator.sendUdpMessage("AD+", serverIp, serverPort);
            },
            onTapUp: (_) {
              communicator.sendUdpMessage("AD", serverIp, serverPort);
            },
            child: Container(
              margin: EdgeInsets.all(20),
              padding: EdgeInsets.symmetric(horizontal: 40, vertical: 20),
              color: Colors.blue,
              child: Text(
                'AD+',
                style: TextStyle(color: Colors.white, fontSize: 18),
              ),
            ),
          ),
          ],),

           GestureDetector(
            onTapDown: (_) {
              communicator.sendUdpMessage("DEC-", serverIp, serverPort);
            },
            onTapUp: (_) {
              communicator.sendUdpMessage("DEC", serverIp, serverPort);
            },
            child: Container(
              margin: EdgeInsets.all(20),
              padding: EdgeInsets.symmetric(horizontal: 40, vertical: 20),
              color: Colors.blue,
              child: Text(
                'DEC-',
                style: TextStyle(color: Colors.white, fontSize: 18),
              ),
            ),
          ),
            SizedBox(height: 20), // Espacement avant la zone de débogage
          // Champ de texte multiligne pour afficher les messages de debug
          Expanded(
            child: Container(
              padding: EdgeInsets.all(10.0),
              color: Colors.black12,
              child: SingleChildScrollView(
                child: Text(
                  debugMessages.join("\n"),
                  style: TextStyle(fontSize: 14, color: Colors.black87),
                ),
              ),
            ),
          )          
          ],
        ),
      ),
    );
  }
}
