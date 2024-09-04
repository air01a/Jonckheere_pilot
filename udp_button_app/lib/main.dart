import 'package:flutter/material.dart';
import 'dart:io'; // Pour la gestion du socket UDP
import 'dart:convert'; // Pour encoder les chaînes de caractères

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
  final String serverIp = "192.168.1.1";  // Remplacez par l'adresse IP de votre serveur
  final int serverPort = 4000;
  List<String> debugMessages = [];
  bool udp=false;


   Future<void>  sendUdpMessage(String message) async {
    try {
      RawDatagramSocket.bind(InternetAddress.anyIPv4, 0).then((socket) {
        List<int> data = utf8.encode(message);
        addDebugMessage('Envoi du message : $message');
        socket.send(data, InternetAddress(serverIp), serverPort);
        socket.close();
        addDebugMessage('Message envoyé avec succès.');
      }).catchError((error) {
        addDebugMessage('Erreur lors de l\'envoi : $error');
      });
    } catch (e) {
      addDebugMessage('Exception : $e');
    }
  }

Future<void> sendTcpMessage(String message) async {
    try {
      // Établir une connexion TCP avec le serveur
      Socket socket = await Socket.connect(serverIp, serverPort);
      addDebugMessage('Connexion TCP établie avec le serveur.');

      // Envoyer le message au serveur
      socket.write(message);
      addDebugMessage('Message envoyé : $message');

      // Fermer la connexion TCP
      socket.close();
      addDebugMessage('Connexion TCP fermée.');
    } catch (e) {
      addDebugMessage('Erreur TCP : $e');
    }
  }


  
Future<void> sendMessage(String message) async {
    if (udp==true) {
      await  sendUdpMessage(message);
    } else {
      await  sendTcpMessage(message);
    }
  }



  // Fonction pour ajouter des messages à l'interface de débogage
  void addDebugMessage(String message) {
    setState(() {
      debugMessages.add(message);
    });
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
            Row(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Text('TCP'),
              Switch(
                value: udp,
                onChanged: (bool value) {
                  setState(() {
                    udp = value;  // Mettre à jour l'état du booléen
                    addDebugMessage('Mode changé : ${udp ? "UDP" : "TCP"}');
                  });
                },
              ),
              Text('UDP'),
            ],
          ),
            // Première ligne avec 3 boutons
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: [
                ElevatedButton(
                  onPressed: () {
                    sendMessage("sidereal");
                  },
                  child: Text('Sidereal'),
                ),
                ElevatedButton(
                  onPressed: () {
                    sendMessage("solar");
                  },
                  child: Text('Solar'),
                ),
                ElevatedButton(
                  onPressed: () {
                    sendMessage("lunar");
                  },
                  child: Text('lunar'),
                ),
              ],
            ),
            SizedBox(height: 20), // Espacement entre les lignes
            // Deuxième ligne avec 4 boutons
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: [
                ElevatedButton(
                  onPressed: () {
                    sendMessage("x1");
                  },
                  child: Text('x1'),
                ),
                ElevatedButton(
                  onPressed: () {
                    sendMessage("x2");
                  },
                  child: Text('x2'),
                ),
                ElevatedButton(
                  onPressed: () {
                    sendMessage("x4");
                  },
                  child: Text('x4'),
                ),
                ElevatedButton(
                  onPressed: () {
                    sendMessage("x16");
                  },
                  child: Text('x16'),
                ),
              ],
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
