from google.cloud import bigquery

client = bigquery.Client.from_service_account_json('/Users/orbiszeus/Downloads/linear-aviary-420617-4ec57e3a8c4e.json')

# dataset_id = 'linear-aviary-420617.iotDB'
# table_id = 'linear-aviary-420617.iotDB.Flight_Data'

# table_ref = client.dataset(dataset_id).table(table_id)
# table = client.get_table(table_ref)

# Perform a query.
QUERY = (
    'SELECT ST_MAKELINE(ST_GEOGPOINT(longitude1, latitude1), ST_GEOGPOINT(longitude2, latitude2)) AS line, '
    'longitude1 AS start_longitude, '
    'latitude1 AS start_latitude, '
    'longitude2 AS end_longitude, '
    'latitude2 AS end_latitude, '
    'call_sign AS call_sign '
    'FROM `linear-aviary-420617.iotDB.Flight_Data` '
    'WHERE velocity >= 60 '
    'LIMIT 100'
)
query_job = client.query(QUERY)  # API request

rows = query_job.result()  # Waits for query to finish

for row in rows:
    # Accessing elements by index
    line = row[0]
    start_longitude = row[1]
    start_latitude = row[2]
    end_longitude = row[3]
    end_latitude = row[4]
    call_sign = row[5]

    # Accessing elements by field names from the dictionary
    line = row['line']
    start_longitude = row['start_longitude']
    start_latitude = row['start_latitude']
    end_longitude = row['end_longitude']
    end_latitude = row['end_latitude']
    call_sign = row['call_sign']

    # Print the values
    print("Line:", line)
    print("Start Longitude:", start_longitude)
    print("Start Latitude:", start_latitude)
    print("End Longitude:", end_longitude)
    print("End Latitude:", end_latitude)
    print("Call Sign:", call_sign)

        