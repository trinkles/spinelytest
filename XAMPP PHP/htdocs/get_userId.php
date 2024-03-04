<?php
// Database connection and configuration
$servername = "localhost";
$username_db = "root"; // Change this to your database username
$password_db = ""; // Change this to your database password
$dbname = "spinelytest"; // Change this to your database name

// Create connection
$conn = new mysqli($servername, $username_db, $password_db, $dbname);

// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

// Fetch userId based on the provided username
if ($_SERVER["REQUEST_METHOD"] == "POST") {
  $loginUsername = $_POST['loginUsername']; // Retrieve username from POST parameter

  // Prepare and bind SQL statement
  $stmt = $conn->prepare("SELECT userId FROM users WHERE username = ?");
  $stmt->bind_param("s", $loginUsername);

  // Execute SQL statement
  $stmt->execute();

  // Get result
  $result = $stmt->get_result();

  // Check if user exists
  if ($result->num_rows > 0) {
    // User found, return userId
    $row = $result->fetch_assoc();
    $userId = $row['userId'];
    echo $userId; // Return userId directly
  } else {
    // User not found
    echo "User not found";
  }

  // Close prepared statement
  $stmt->close();
}

// Close database connection
$conn->close();
?>
