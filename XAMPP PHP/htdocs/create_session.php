<?php
// Check if the request method is POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Retrieve userId from the POST request
    $userId = $_POST["userId"];

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

    // Prepare the SQL query to insert a new row in the sessions table
    $sql = "INSERT INTO sessions (userId) VALUES ('$userId')";

    // Execute the SQL query
    if ($conn->query($sql) === TRUE) {
        // Retrieve the sessionId of the newly created session
        $sessionId = $conn->insert_id;
        echo $sessionId; // Return the sessionId
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }

    // Close the database connection
    $conn->close();
} else {
    echo "Invalid request method";
}
?>
