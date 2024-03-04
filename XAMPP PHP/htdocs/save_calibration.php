<?php
// Check if the request method is POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Retrieve calibration data and user ID from the POST request
    $userId = $_POST["userId"]; // Assuming userId is passed in the POST request
    $sensor1 = $_POST["sensor1"];
    $sensor2 = $_POST["sensor2"];
    $sensor3 = $_POST["sensor3"];
    $sensor4 = $_POST["sensor4"];
    $sensor5 = $_POST["sensor5"];
    $sensor6 = $_POST["sensor6"];
    $sensor7 = $_POST["sensor7"];
    $sensor8 = $_POST["sensor8"];
    $sensor9 = $_POST["sensor9"];
    $sensor10 = $_POST["sensor10"];
    $sensor11 = $_POST["sensor11"];
    $sensor12 = $_POST["sensor12"];
    $sensor13 = $_POST["sensor13"];
    $sensor14 = $_POST["sensor14"];
    $sensor15 = $_POST["sensor15"];

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

    // Check if a row with the specified userId already exists in the device_calibration table
    $sql_check = "SELECT * FROM device_calibration WHERE userId = '$userId'";
    $result = $conn->query($sql_check);

    if ($result->num_rows > 0) {
        // If a row exists, update the existing row
        $sql_update = "UPDATE device_calibration SET cervical_min_angle='$sensor1', cervical_max_angle='$sensor2', cervical_avg_angle='$sensor3', thoracic_min_angle='$sensor4', thoracic_max_angle='$sensor5', thoracic_avg_angle='$sensor6', lumbar_min_angle='$sensor7', lumbar_max_angle='$sensor8', lumbar_avg_angle='$sensor9', left_midAxLine_min_angle='$sensor10', left_midAxLine_max_angle='$sensor11', left_midAxLine_avg_angle='$sensor12', right_midAxLine_min_angle='$sensor13', right_midAxLine_max_angle='$sensor14', right_midAxLine_avg_angle='$sensor15' WHERE userId='$userId'";
        if ($conn->query($sql_update) === TRUE) {
            echo "Calibration data updated successfully";
        } else {
            echo "Error updating record: " . $conn->error;
        }
    } else {
        // If no row exists, insert a new row
        $sql_insert = "INSERT INTO device_calibration (userId, cervical_min_angle, cervical_max_angle, cervical_avg_angle, thoracic_min_angle, thoracic_max_angle, thoracic_avg_angle, lumbar_min_angle, lumbar_max_angle, lumbar_avg_angle, left_midAxLine_min_angle, left_midAxLine_max_angle, left_midAxLine_avg_angle, right_midAxLine_min_angle, right_midAxLine_max_angle, right_midAxLine_avg_angle) VALUES ('$userId', '$sensor1', '$sensor2', '$sensor3', '$sensor4', '$sensor5', '$sensor6', '$sensor7', '$sensor8', '$sensor9', '$sensor10', '$sensor11', '$sensor12', '$sensor13', '$sensor14', '$sensor15')";
        if ($conn->query($sql_insert) === TRUE) {
            echo "New record created successfully";
        } else {
            echo "Error: " . $sql_insert . "<br>" . $conn->error;
        }
    }

    // Close the database connection
    $conn->close();
} else {
    echo "Invalid request method";
}
?>
