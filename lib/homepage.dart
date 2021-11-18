import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:percent_indicator/circular_percent_indicator.dart';

class HomePage extends StatefulWidget {
  const HomePage({Key? key}) : super(key: key);

  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  final _databaseReference = FirebaseDatabase.instance.reference();

  @override
  Widget build(BuildContext context) {
    return SafeArea(
      child: Scaffold(
        appBar: AppBar(
          backgroundColor: Colors.black,
          elevation: 0,
          title: const Text("Home"),
        ),
        body: Container(
          height: MediaQuery.of(context).size.height,
          // color: const Color.fromRGBO(162, 121, 21, 0.5),
          padding: const EdgeInsets.only(top: 30),
          child: StreamBuilder(
            stream: _databaseReference.onValue,
            builder: (context, AsyncSnapshot<Event> snapshot) {
              if (snapshot.hasData) {
                var data = snapshot.data!.snapshot.value;
                return _buildBody(
                  temperature: data['Temperature'],
                  humidity: data['Humidity'],
                  moisture: data['Soil_Value'],
                  context: context,
                );
              } else {
                return const Center(
                  child: CircularProgressIndicator(
                    color: Colors.black,
                  ),
                );
              }
            },
          ),
        ),
      ),
    );
  }
}

Widget _buildBody(
    {required dynamic temperature,
    required dynamic humidity,
    required dynamic moisture,
    required BuildContext context}) {
  return Container(
    child: Column(
      children: [
        Row(
          mainAxisAlignment: MainAxisAlignment.spaceAround,
          mainAxisSize: MainAxisSize.min,
          children: [
            _buildTemperatureIndicator(
              text: "Temperature",
              value: temperature,
              context: context,
            ),
            _buildContainer(
              text: "Humidity",
              value: humidity,
              context: context,
              color: Colors.blue,
            ),
          ],
        ),
        const SizedBox(
          height: 20,
        ),
        _buildContainer(
          text: "Moisture",
          value: moisture,
          context: context,
          color: Colors.orange,
        )
      ],
    ),
  );
}

Widget _buildContainer({
  required String text,
  required dynamic value,
  required BuildContext context,
  required Color color,
}) {
  bool check = text == "Humidity" || text == "Moisture";

  return Container(
    width: MediaQuery.of(context).size.width * 0.5,
    child: ListTile(
      title: CircularPercentIndicator(
        animation: true,
        animationDuration: 1500,
        animateFromLastPercent: true,
        radius: 170.0,
        lineWidth: 15,
        percent: check ? value / 100 : 1.0,
        center: Text(
          check ? "${value} %" : "${value} °C",
          style: const TextStyle(
            fontSize: 40,
            fontWeight: FontWeight.bold,
          ),
        ),
        progressColor: color,
      ),
      subtitle: Center(
        child: Container(
          margin: const EdgeInsets.only(top: 10),
          child: Text(
            text,
            style: const TextStyle(
              fontSize: 18,
              fontWeight: FontWeight.bold,
            ),
          ),
        ),
      ),
    ),
  );
}

Widget _buildTemperatureIndicator({
  required String text,
  required dynamic value,
  required BuildContext context,
}) {
  if (value < 15) {
    return _buildContainer(
      text: text,
      value: value,
      context: context,
      color: Colors.deepPurple,
    );
  } else if (value >= 15 && value <= 45) {
    return _buildContainer(
      text: text,
      value: value,
      context: context,
      color: Colors.green,
    );
  } else {
    return _buildContainer(
      text: text,
      value: value,
      context: context,
      color: Colors.red,
    );
  }
}
