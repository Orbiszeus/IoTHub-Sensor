<template>
  <div class="card">
    <div class="p-3 pb-0 card-header">
      <h6 class="mb-0">{{ cardTitle }}</h6>
    </div>
    <div class="p-3 card-body">
      <ul class="list-group" :class="this.$store.state.isRTL ? 'pe-0' : ''">
        <li class="mb-2 border-0 list-group-item d-flex justify-content-between border-radius-lg"
          :class="this.$store.state.isRTL ? 'pe-0' : 'ps-0'">
          <div class="d-flex align-items-center">
            <div class="text-center shadow icon icon-shape icon-sm bg-gradient-dark"
              :class="this.$store.state.isRTL ? 'ms-3' : 'me-3'">
              <i class="fa fa-thermometer-half text-white opacity-10"></i>
            </div>
            <div class="d-flex flex-column">
              <h6 class="mb-1 text-sm text-dark">{{ localTitleDesc }}</h6>
              <span class="text-xs">
                {{ localTitle }},
                <span class="font-weight-bold">{{ titleDesc2 }}</span>
              </span>
            </div>
          </div>
          <div class="d-flex">
            <button class="my-auto btn btn-link btn-icon-only btn-rounded btn-sm text-dark icon-move-right">
              <i class="ni ni-bold-right" aria-hidden="true"></i>
            </button>
          </div>
        </li>
        <li class="mb-2 border-0 list-group-item d-flex justify-content-between border-radius-lg"
          :class="this.$store.state.isRTL ? 'pe-0' : 'ps-0'">
          <div class="d-flex align-items-center">
            <div class="text-center shadow icon icon-shape icon-sm bg-gradient-dark"
              :class="this.$store.state.isRTL ? 'ms-3' : 'me-3'">
              <i class="ni ni-mobile-button opacity-10"></i>
            </div>
            <div class="d-flex flex-column">
              <h6 class="mb-1 text-sm text-dark">{{ localTitleDesc2 }}</h6>
              <span class="text-xs">
                {{ localTitle2 }},
                <span class="font-weight-bold">{{ title2Desc2 }}</span>
              </span>
            </div>
          </div>
          <div class="d-flex">
            <button class="my-auto btn btn-link btn-icon-only btn-rounded btn-sm text-dark icon-move-right">
              <i class="ni ni-bold-right" aria-hidden="true"></i>
            </button>
          </div>
        </li>
        <li class="border-0 list-group-item d-flex justify-content-between border-radius-lg"
          :class="this.$store.state.isRTL ? 'pe-0' : 'ps-0'">
          <div class="d-flex align-items-center">
            <div class="text-center shadow icon icon-shape icon-sm bg-gradient-dark"
              :class="this.$store.state.isRTL ? 'ms-3' : 'me-3'">
              <i class="fa fa-lightbulb-o text-white opacity-10"></i>
            </div>
            <div class="d-flex flex-column">
              <h6 class="mb-1 text-sm text-dark">{{ localTitleDesc3 }}</h6>
              <span class="text-xs">
                {{ localTitle3 }},
                <span class="font-weight-bold">{{ title3Desc2 }}</span>
              </span>
            </div>
          </div>
          <div class="d-flex">
            <button class="my-auto btn btn-link btn-icon-only btn-rounded btn-sm text-dark icon-move-right">
              <i class="ni ni-bold-right" aria-hidden="true"></i>
            </button>
          </div>
        </li>
        <li class="border-0 list-group-item d-flex justify-content-between border-radius-lg"
          :class="this.$store.state.isRTL ? 'pe-0' : 'ps-0'">
          <div class="d-flex align-items-center">
            <div class="text-center shadow icon icon-shape icon-sm bg-gradient-dark"
              :class="this.$store.state.isRTL ? 'ms-3' : 'me-3'">
              <i class="fa fa-keyboard-o text-white opacity-10"></i>
            </div>
            <div class="d-flex flex-column">
              <h6 class="mb-1 text-sm text-dark">{{ localTitleDesc4 }}</h6>
              <span class="text-xs font-weight-bold">{{ localTitle4 }}</span>
            </div>
          </div>
          <div class="d-flex">
            <button class="my-auto btn btn-link btn-icon-only btn-rounded btn-sm text-dark icon-move-right">
              <i class="ni ni-bold-right" aria-hidden="true"></i>
            </button>
          </div>
        </li>
      </ul>
    </div>
  </div>
</template>

<script>
import axios from 'axios';

export default {
  name: "categories-card",
  data() {
    return {
      localTitleDesc: this.titleDesc,
      localTitle: this.title,
      localTitleDesc2: this.titleDesc2,
      localTitle2: this.title2,
      localTitleDesc3: this.titleDesc3,
      localTitle3: this.title3,
      localTitleDesc4: this.titleDesc4,  // Initialize a local property with the value of the prop
      localTitle4: this.title4,

    };
  },
  props: {
    cardTitle: {
      type: String,
      default: "Recent Alerts",
    },
    title: {
      type: String,
      default: "Devices",
    },
    title2: {
      type: String,
      default: "Tickets",
    },
    title3: {
      type: String,
      default: "Error logs",
    },
    title4: {
      type: String,
      default: "",
    },
    titleDesc: {
      type: String,
      default: "",
    },
    titleDesc2: {
      type: String,
      default: "",
    },
    title2Desc: {
      type: String,
      default: "",
    },
    title4Desc: {
      type: String,
      default: "",
    },
    title2Desc2: {
      type: String,
      default: "",
    },
    title3Desc: {
      type: String,
      default: "1 is active",
    },
    title3Desc2: {
      type: String,
      default: "40 closed",
    },
  },
  methods: {
    getTypeIconDesc(deviceType){
      let iconType = 'fa fa-keyboard-o text-white opacity-10'
      switch (deviceType){
        case 'Thermometer': iconType = 'fa fa-keyboard-o text-white opacity-10'
                break;
        case 'Light_sensor': iconType = 'fa fa-keyboard-o text-white opacity-10'
                break;
        case 'Screen': iconType = 'fa fa-keyboard-o text-white opacity-10'
                break;
        case 'Keyboard': iconType = 'fa fa-keyboard-o text-white opacity-10'
                break;
        default: iconType = 'fa fa-keyboard-o text-white opacity-10'
                break;
      }
      return iconType;
    }
  },
  created() {
    axios.get('http://127.0.0.1:8000/count-types')
      .then(response => {
        const data = response.data[0]; // Assuming you want to use the first object in the array
        console.debug(data.count)
        // TODO: Add a method to identify the type and place it correctly to the UI 
        // switch 
        this.localTitleDesc = `Count: ${data.count}`;
        this.localTitle = data.type;
        this.localTitleDesc2 = `Count: ${response.data[1].count}`;
        this.localTitle2 = response.data[1].type;
        this.localTitleDesc3 = `Count: ${response.data[2].count}`;
        this.localTitle3 = response.data[2].type;
        this.localTitleDesc4 = `Count: ${response.data[3].count}`;
        this.localTitle4 = response.data[3].type;

        //  {
        //   title: "Count: ",
        //   // value: this.secureMAC(data.device_mac),
        //   value: data.count,

        // };
      })
      .catch(error => {
        console.error(error);
      });
  }
};
</script>
