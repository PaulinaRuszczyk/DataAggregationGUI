# Description
Software that illustrates data on the inventory of individual products. It also enables adding new items to collect data and deleting them. Soon it will enables order processing and showing its impact on stock in stores.
# Requirements
   * g++ 
   * AWS SDK C++ 
   * CMake Minimum version 3.13
   * DynamoDB database
   * Qt 4.0
   
# Configuration
Software is configured using Credentials.env file located in build folder containing fields: 

```accessKeyId:YourAwsAccessKey```

```secretKey:YourAwsAccessKy```

# Usage

Software uses 2 databases:
* First one called names including 
  - id (Primary Key) - id of an object 
  - name - name of an object whose data are to be collected.
* Second one called quantities which is an output of an app includes 
  * lp (Primary key)
  * id - id of an object
  * quantity
  * date of recording.

Software shows lists of products saved in database and its data on the inventory
stock piles over time in form of an interactive graph.  It is possible to delete or add new product to track data.
