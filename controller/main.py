from google.cloud import bigquery

client = bigquery.Client.from_service_account_json('/Users/orbiszeus/Downloads/IOT_Final_KEY.json')

# dataset_id = 'linear-aviary-420617.iotDB'
# table_id = 'linear-aviary-420617.iotDB.Flight_Data'

# table_ref = client.dataset(dataset_id).table(table_id)
# table = client.get_table(table_ref)

# Perform a query.
QUERY = (
    'SELECT * FROM local-incline-419216.finalproject.finalproject LIMIT 100'
)
query_job = client.query(QUERY)  # API request

rows = query_job.result()  # Waits for query to finish
counter = 0
for row in rows:
    # Accessing elements by index
    temperature = row[0]
    humidity = row[1]
    co = row[2]
    alcohol = row[3]
    co2 = row[4]
    tolueno = row[5]
    nh4 = row[6]
    acetona = row[7]
    date = row[8]
    id = row[9]   

    # Accessing elements by field names from the dictionary
    temperature = row['temperature']
    humidity = row['humidity']
    co = row['co']
    alcohol = row['alcohol']
    co2 = row['co2']
    tolueno = row['Tolueno']
    nh4 = row['NH4']
    acetona = row['Acetona']
    date = row['when']
    id = row['ID_PLACA']

    # Print the values
    
    print("Temperature: ", temperature)
    print("Humidity: ", humidity)
    print("Carbon Monoxide: ", co)
    print("Carbon Dioxide: ", co2)
    print("Tolueno: ", tolueno)
    print("Amoniac: ", nh4)
    print("Acetone: ", acetona)
    print("Date: ", date)
    print("ID: ", id)
    counter += 1 
    print("Row: " ,counter)
    print("-----------------------------------")
        