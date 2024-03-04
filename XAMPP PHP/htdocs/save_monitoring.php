<?php
// Check if the request method is POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Retrieve data from the POST request
    $userId = $_POST["userId"];
    $sessionId = $_POST["sessionId"];
    $status = $_POST["status"];
    $sensor1 = $_POST["sensor1"];
    $sensor2 = $_POST["sensor2"];
    $sensor3 = $_POST["sensor3"];
    $sensor4 = $_POST["sensor4"];
    $sensor5 = $_POST["sensor5"];

    // Create a connection to the MySQL database
    $servername = "localhost"; // Change this to your MySQL server hostname
    $username = "root"; // Change this to your MySQL username
    $password = ""; // Change this to your MySQL password
    $dbname = "spinelytest"; // Change this to your MySQL database name

    $conn = new mysqli($servername, $username, $password, $dbname);

    // Check if the connection was successful
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    // Prepare the SQL query to insert monitoring data into the database
    $sql = "INSERT INTO monitoring (userId, sessionId, postureStatus, cervical, thoracic, lumbar, leftMidAx, rightMidAx)
            VALUES ('$userId', '$sessionId', '$status', '$sensor1', '$sensor2', '$sensor3', '$sensor4', '$sensor5')";

    // Execute the SQL query
    if ($conn->query($sql) === TRUE) {
        echo "Monitoring data saved successfully";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }

    // Close the database connection
    $conn->close();
} else {
    echo "Invalid request method";
}
?>
