from google.cloud import bigquery
import json

client = bigquery.Client.from_service_account_json('/Users/orbiszeus/Downloads/IOT_Final_KEY.json')

class Repository:
        
    def get_all_items(self, limit):
        all_items = []
        print("hey")
        QUERY = (
            'SELECT * FROM local-incline-419216.finalproject.finalproject '
            'ORDER BY `when` DESC LIMIT @limit'
        )
        
        job_config = bigquery.QueryJobConfig(
            query_parameters=[
            bigquery.ScalarQueryParameter('limit', 'INT64', limit)])
        
        query_job = client.query(QUERY, job_config=job_config)

        rows = query_job.result() 
        
        for row in rows:
        
            item = {
                "temperature": row['temperature'],
                "humidity": row['humidity'],
                "co": row['co'],
                "alcohol": row['alcohol'],
                "co2": row['co2'],
                "tolueno": row['Tolueno'],
                "nh4": row['NH4'],
                "acetona": row['Acetona'],
                "date": row['when'].isoformat(),
                "id": row['ID_PLACA']
            }
        
            all_items.append(item)
        
        all_items_json = json.dumps(all_items)
        
        return all_items_json

    
    def get_latest_item(self):
        QUERY = (
            'SELECT * FROM `local-incline-419216.finalproject.finalproject` '
            'ORDER BY `when` DESC LIMIT 1'
        )
        query_job = client.query(QUERY)  

        rows = query_job.result() 
        
        row = next(rows)
        item = {
            "temperature": row['temperature'],
            "humidity": row['humidity'],
            "co": row['co'],
            "alcohol": row['alcohol'],
            "co2": row['co2'],
            "tolueno": row['Tolueno'],
            "nh4": row['NH4'],
            "acetona": row['Acetona'],
            "date": row['when'].isoformat(),  # Convert datetime to string
            "id": row['ID_PLACA']
        }

        # Convert the dictionary to a JSON object without indentation
        item_json = json.dumps(item)
        
        return item_json
