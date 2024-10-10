
import 'dart:io'; // Pour la gestion du socket UDP
import 'dart:convert'; // Pour encoder les chaînes de caractères

class Communicator {

  final Function preSendFunction;
  final Function postSendFunction;
  final Function debug;
  RawDatagramSocket? socket;

  Communicator({required this.preSendFunction, required this.postSendFunction, required this.debug});

  Future<void> sendUdpMessage(String message, String serverIp, int serverPort) async {
    preSendFunction();
    try {
      // Créer un socket UDP
      RawDatagramSocket.bind(InternetAddress.anyIPv4, 0).then((socket) {
        List<int> data = utf8.encode(message);
        debug('Envoi du message : $message');
        socket.send(data, InternetAddress(serverIp), serverPort);

        // Écouter la réponse du serveur
        socket.listen((RawSocketEvent event) {
          if (event == RawSocketEvent.read) {
            Datagram? datagram = socket.receive();
            if (datagram != null) {
              String response = utf8.decode(datagram.data);
              debug('Réponse reçue : $response');

              // Vérifier si la réponse est "OK"
              if (response.trim() == 'OK') {
                debug('Réponse du serveur : OK');
              } else {
                debug('Réponse inattendue : $response');
              }
              postSendFunction(response);

            }
          }
        });
      }).catchError((error) {
        debug('Erreur lors de l\'envoi : $error');
      });
    } catch (e) {
      debug('Exception : $e');
    }
  }

  Future<void> sendTcpMessage(String message, String serverIp, int serverPort) async {
    preSendFunction();

    try {
      // Établir une connexion TCP avec le serveur
      Socket socket = await Socket.connect(serverIp, serverPort);
      debug('Connexion TCP établie avec le serveur.');

      // Envoyer le message au serveur
      socket.write(message);
      debug('Message envoyé : $message');

      // Lire la réponse du serveur
      socket.listen((data) {
        String response = utf8.decode(data);
        debug('Réponse reçue : $response');

        // Vérifier si la réponse est "OK"
        if (response.trim() == 'OK') {
          debug('Réponse du serveur : OK');
        } else {
          debug('Réponse inattendue : $response');
        }

        // Fermer la connexion
        socket.destroy();
        debug('Connexion TCP fermée.');
        postSendFunction(response);
      });
    } catch (e) {
      debug('Erreur TCP : $e');
    }
  }


  void onPacketReceived(String message, InternetAddress senderAddress) {
    print('Paquet reçu: $message de $senderAddress');
    // Place ici la logique à exécuter lorsque le paquet est reçu
  }

  // Démarre l'écoute UDP avec RawDatagramSocket
  Future<void> startUdpListening(Function callback) async {
    // Crée un RawDatagramSocket lié au port 4000
    RawDatagramSocket.bind(InternetAddress.anyIPv4, 4001).then((RawDatagramSocket udpSocket) {
      socket = udpSocket;

      // Ecoute les événements sur le socket
      udpSocket.listen((RawSocketEvent event) {
        if (event == RawSocketEvent.read) {
          Datagram? datagram = udpSocket.receive();
          if (datagram != null) {
            String message = utf8.decode(datagram.data);
            //InternetAddress senderAddress = datagram.address;
            callback(message);
            // Appelle la fonction à l'arrivée du paquet
            //onPacketReceived(message, senderAddress);
          }
        }
      });
    });
  }

}