<template>
  <div class="py-4 container-fluid">
    <div class="row">
      <div class="col-lg-12">
        <div class="row">
          <div class="col-lg-3 col-md-6 col-12">
            <card :title="stats.money.title" :value="stats.money.value" :percentage="stats.money.percentage"
              :iconClass="stats.money.iconClass" :iconBackground="stats.money.iconBackground" :detail="stats.money.detail"
              directionReverse></card>
          </div>
          <div class="col-lg-3 col-md-6 col-12">
            <card :title="stats.users.title" :value="stats.users.value" :percentage="stats.users.percentage"
              :iconClass="stats.users.iconClass" :iconBackground="stats.users.iconBackground" :detail="stats.users.detail"
              directionReverse></card>
          </div>
          <div class="col-lg-3 col-md-6 col-12">
            <card :title="stats.clients.title" :value="stats.clients.value" :percentage="stats.clients.percentage"
              :iconClass="stats.clients.iconClass" :iconBackground="stats.clients.iconBackground"
              :percentageColor="stats.clients.percentageColor" :detail="stats.clients.detail" directionReverse></card>
          </div>
          <div class="col-lg-3 col-md-6 col-12">
            <card :title="stats.sales.title" :value="stats.sales.value" :percentage="stats.sales.percentage"
              :iconClass="stats.sales.iconClass" :iconBackground="stats.sales.iconBackground" :detail="stats.sales.detail"
              directionReverse></card>
          </div>
        </div>
        <div class="row">
          <div class="col-lg-7 mb-lg">
            <!-- line chart -->
            <div class="card z-index-2">
              <gradient-line-chart />
            </div>
          </div>
          <div class="col-lg-5">
            <carousel />
          </div>
        </div>
        <div class="row mt-4">
          <div class="col-lg-6 mb-lg-0 mb-4">
            <div class="card">
              <div class="card-body">
                <h5 class="card-title">Control LED</h5>
                <div class="form-group">
                  <label for="colorSelect">Select LED Color:</label>
                  <select v-model="selectedColor" class="form-control" id="colorSelect">
                    <option value="red">Red</option>
                    <option value="green">Green</option>
                    <option value="yellow">Yellow</option>
                  </select>
                </div>
                <button @click="changeLedState(1)" class="btn btn-success mr-2">On</button>
                <button @click="changeLedState(0)" class="btn btn-danger">Off</button>
              </div>
            </div>
          </div>
        </div>
        <div class="row mt-4">
          <div class="col-lg-7 mb-lg-0 mb-4">
            <!-- Placeholder for other content -->
          </div>
          <div class="col-lg-5">
            <categories-card />
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios';
import Card from "@/examples/Cards/Card.vue";
import GradientLineChart from "@/examples/Charts/GradientLineChart.vue";
import Carousel from "./components/Carousel.vue";
import CategoriesCard from "./components/CategoriesCard.vue";

import US from "@/assets/img/icons/flags/US.png";
import DE from "@/assets/img/icons/flags/DE.png";
import GB from "@/assets/img/icons/flags/GB.png";
import BR from "@/assets/img/icons/flags/BR.png";

export default {
  name: "dashboard-default",
  data() {
    return {
      stats: {
        money: {},
        users: {},
        clients: {},
        sales: {},
      },
      sales: {
        us: {
          country: "United States",
          sales: 2500,
          value: "$230,900",
          bounce: "29.9%",
          flag: US,
        },
        germany: {
          country: "Germany",
          sales: "3.900",
          value: "$440,000",
          bounce: "40.22%",
          flag: DE,
        },
        britain: {
          country: "Great Britain",
          sales: "1.400",
          value: "$190,700",
          bounce: "23.44%",
          flag: GB,
        },
        brasil: {
          country: "Brasil",
          sales: "562",
          value: "$143,960",
          bounce: "32.14%",
          flag: BR,
        },
      },
      selectedColor: 'red',  // Default selected color
      alerts: [],  // To store alerts
      intervalId: null  // ID for the interval timer
    };
  },
  components: {
    Card,
    GradientLineChart,
    Carousel,
    CategoriesCard,
  },
  methods: {
    secureMAC(mac) {
      let securedMAC = mac;
      if (mac.length > 20)
        securedMAC = mac.slice(0, 8) + '...' + mac.slice(mac.length - 8, mac.length);
      return securedMAC;
    },
    async changeLedState(state) {
      let endpoint = '';
      switch (this.selectedColor) {
        case 'red':
          endpoint = 'change_red_led';
          break;
        case 'green':
          endpoint = 'change_green_led';
          break;
        case 'yellow':
          endpoint = 'change_yellow_led';
          break;
      }
      try {
        const response = await axios.get(`http://127.0.0.1:8000/${endpoint}?on_off=${state}`);
        console.log(response.data);
      } catch (error) {
        console.error('Error:', error);
      }
    },
    async fetchAlerts() {
      try {
        const response = await axios.get('http://127.0.0.1:8000/alerts');
        const alertData = response.data;
        if (alertData.temperature === 1) {
          alert('High Temperature Alert!!');
        }
        if (alertData.humidity === 1) {
          alert('High Humidity Alert!!');
        }
        if (alertData.alcohol === 1) {
          alert('High Alcohol Alert!!');
        }
      } catch (error) {
        console.error('Error fetching alerts:', error);
      }
    },
  },
  created() {
    axios.get('http://127.0.0.1:8000/latest_item')
      .then(response => {
        const data = JSON.parse(response.data);
        this.stats.money = {
          title: "ID",
          value: data.id,
          iconClass: "ni ni-money-coins",
          iconBackground: "bg-gradient-primary",
        };
        this.stats.users = {
          title: "Temperature",
          value: data.temperature,
          iconClass: "ni ni-world",
          iconBackground: "bg-gradient-danger",
        };
        this.stats.clients = {
          title: "Humidity",
          value: data.humidity,
          iconClass: "ni ni-paper-diploma",
          iconBackground: "bg-gradient-success",
        };
        this.stats.sales = {
          title: "Date",
          value: data.date.toString().slice(0, 10),
          iconClass: "ni ni-cart",
          iconBackground: "bg-gradient-warning",
        };
      })
      .catch(error => {
        console.error(error);
      });

    // Poll the alerts endpoint every 5 seconds
    this.intervalId = setInterval(this.fetchAlerts, 5000);
  },
  beforeUnmount() {
    // Clear the interval when the component is destroyed
    if (this.intervalId) {
      clearInterval(this.intervalId);
    }
  }
};
</script>

<style scoped>
.col-lg-7 {
  width: 100%;
}

.col-lg-5 {
  padding-top: 35px;
  width: 100%;
}

.card-title {
  font-weight: bold;
  margin-bottom: 20px;
}

.form-group {
  margin-bottom: 20px;
}

.btn {
  margin-right: 10px;
}
</style>
