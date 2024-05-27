from fastapi import FastAPI, HTTPException
import repository
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

repo = repository.Repository

@app.get("/all_items")
def get_all():
     
    return repo.get_all_items()

@app.get("/latest_item")
def get_latest():
    print(repo.get_latest_item()) 
    return repo.get_latest_item()

