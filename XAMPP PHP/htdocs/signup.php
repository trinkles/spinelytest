<?php
// Check if the request method is POST
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Retrieve signup data from the POST request
    $name = $_POST["name"];
    $user_username = $_POST["username"]; // Use a different variable name here
    $email = $_POST["email"];

    // Create a connection to the MySQL database
    $servername = "localhost"; // Change this to your MySQL server hostname
    $db_username = "root"; // Change this to your MySQL username
    $password = ""; // Change this to your MySQL password
    $dbname = "spinelytest"; // Change this to your MySQL database name

    $conn = new mysqli($servername, $db_username, $password, $dbname);

    // Check if the connection was successful
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    // Prepare the SQL query to insert signup data into the database
    $sql = "INSERT INTO users (name, username, email, password) VALUES ('$name', '$user_username', '$email', '*****')";

    // Execute the SQL query
    if ($conn->query($sql) === TRUE) {
        echo "New record created successfully";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }

    // Close the database connection
    $conn->close();
} else {
    echo "Invalid request method";
}
?>
