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
          <div class="col-lg-7 mb-lg-0 mb-4">
            
              

                 
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
        money: {

        },
        users: {

        },
        clients: {

        },
        sales: {

        },
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
    }
  },
  created() {
    axios.get('http://127.0.0.1:8000/latest_item')
      .then(response => {
        console.log("hey")
        const data = JSON.parse(response.data);
        console.log(data.when);
        this.stats.money = {
          title: "ID",
          // value: this.secureMAC(data.device_mac),
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
</style>