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
    print(row.name)
