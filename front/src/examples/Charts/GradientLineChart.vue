<template>
  <div class="card">
    <div class="pb-0 card-header mb-0">
      <h6>{{ title }}</h6>
      <p class="text-sm">
        <i class="fa fa-arrow-up text-success"></i>
        <span class="font-weight-bold">{{ detail1 }}</span>
        {{ detail2 }}
      </p>
    </div>
    <div class="p-3 card-body">
      <div class="chart">
        <canvas id="chart-line" class="chart-canvas" height="300"></canvas>
      </div>
    </div>
  </div>
</template>

<script>
import Chart from "chart.js/auto";
import axios from 'axios';
export default {
  name: "gradient-line-chart",
  data() {
    return {
      devices: [],
      temperature_values: [],
      device_types: []
    };
  },
  props: {
    title: {
      type: String,
      default: "Device Temperatures",
    },
    detail1: {
      type: String,
      default: "Temperatures are",
    },
    detail2: {
      type: String,
      default: " show in °C",
    },
  },

  async mounted() {
    await this.fetchData();
    var ctx1 = document.getElementById("chart-line").getContext("2d");

    var gradientStroke1 = ctx1.createLinearGradient(0, 230, 0, 50);

    gradientStroke1.addColorStop(1, "rgba(94, 114, 228, 0.2)");
    gradientStroke1.addColorStop(0.2, "rgba(94, 114, 228, 0.0)");
    gradientStroke1.addColorStop(0, "rgba(94, 114, 228, 0)");
    new Chart(ctx1, {
      type: "line",
      data: {
        labels: this.device_types,
        datasets: [
          {
            label: "Temperature",
            tension: 0.4,
            borderWidth: 0,
            pointRadius: 0,
            borderColor: "#4BB543 ",
            backgroundColor: gradientStroke1,
            // eslint-disable-next-line no-dupe-keys
            borderWidth: 3,
            fill: true,
            data: this.temperature_values,
            maxBarThickness: 6,
          },
        ],
      },
      options: {
        responsive: true,
        maintainAspectRatio: false,
        plugins: {
          legend: {
            display: false,
          },
        },
        interaction: {
          intersect: false,
          mode: "index",
        },
        scales: {
          y: {
            grid: {
              drawBorder: false,
              display: true,
              drawOnChartArea: true,
              drawTicks: false,
              borderDash: [5, 5],
            },
            ticks: {
              display: true,
              padding: 10,
              color: "#fbfbfb",
              font: {
                size: 11,
                family: "Open Sans",
                style: "normal",
                lineHeight: 2,
              },
            },
          },
          x: {
            grid: {
              drawBorder: false,
              display: false,
              drawOnChartArea: false,
              drawTicks: false,
              borderDash: [5, 5],
            },
            ticks: {
              display: true,
              color: "#ccc",
              padding: 20,
              font: {
                size: 11,
                family: "Open Sans",
                style: "normal",
                lineHeight: 2,
              },
            },
          },
        },
      },
    });
  },
  methods: {
    async fetchData() {
      try {
        const response = await axios.get('http://127.0.0.1:8000/all_items');
        const data = JSON.parse(response.data);
        this.devices = data;
        
        console.log(data)

        for (let i = 0; i < this.devices.length; i++) {
        
          this.temperature_values.push(this.devices[i]['temperature']);
          this.device_types.push(this.devices[i]['id'])
          // if (this.devices[i]['type'] == "Thermometer") {
          //   console.log("hello")
          //   this.device_types.push("Device " + i);
          //   // if (this.devices[i]['payload']['data'] == 'Initiating') {
          //   //   this.temperature_values.pop();
          //   //   this.device_types.pop();
          //   // }
          //   // console.log(this.devices[i]['Temperature'])
            
          // }
        }
        console.log(this.temperature_values);

        console.log(this.device_types);
      } catch (error) {
        console.error('Error fetching device data:', error);
      }
    }
  }
};
</script>

<style scoped></style>
