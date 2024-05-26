from fastapi import FastAPI
import repository

app = FastAPI()

repo = repository.Repository

@app.get("/all_items")
def get_all():
     
    return repo.get_all_items()

@app.get("/latest_item")
def get_latest():
     
    return repo.get_latest_item()

