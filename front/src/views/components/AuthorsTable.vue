<template>
  <div class="card">
    <div class="card-header pb-0">
      <h6>Device Information</h6>
    </div>
    <div class="card-body px-0 pt-0 pb-2">
      <div class="table-responsive p-0">
        <table class="table align-items-center mb-0">
          <thead>
            <tr>
              <th id="x" class="text-uppercase text-secondary text-xxs font-weight-bolder opacity-7">Mac Address</th>
              <th class="text-uppercase text-secondary text-xxs font-weight-bolder opacity-7 ps-2">Device Type</th>
              <th class="text-center text-uppercase text-secondary text-xxs font-weight-bolder opacity-7">Device Status
              </th>
              <th class="text-center text-uppercase text-secondary text-xxs font-weight-bolder opacity-7">Hour</th>
              <th class="text-secondary opacity-7"></th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="(device, index) in this.devices" :key="index">
              <td>
                <div class="d-flex px-2 py-1">
                  <div>

                    <!-- <img v-bind:src="'@/assets/img/' + getImageSource(device.type)" class="avatar avatar-sm me-3"
                      alt="user1" /> -->
                      <div class="form-check form-switch">
    <input class="form-check-input" :class="inputClass" type="checkbox" :name="name" :id="id" :checked="checked=device.online" @change="handleCheckboxChange(device.device_mac, $event)"/>
    <label class="form-check-label" :class="labelClass" :for="id">
      <slot />
    </label>
  </div>
                  </div>
                  <div class="d-flex flex-column justify-content-center">
                    <h6 class="mb-0 text-sm">{{ device.device_mac }}</h6>

                  </div>
                </div>
              </td>
              <td>
                <p class="text-xs font-weight-bold mb-0">{{ device.type }}</p>
                <p class="text-xs text-secondary mb-0">{{ device.payload.data }}</p>
              </td>
              <td class="align-middle text-center text-sm">
                <span class="badge badge-sm bg-gradient-success">{{ device.online ? 'Online' : 'Offline'
                }}</span>
              </td>
              <td class="align-middle text-center">
                <span class="text-secondary text-xs font-weight-bold">{{ device.timestamp.toString().slice(0, 9)
                }}</span>
              </td>
              <td class="align-middle">
                <a href="javascript:;" class="text-secondary font-weight-bold text-xs" data-toggle="tooltip"
                  data-original-title="Edit user" @click="diffieHellman(device.device_mac)">DH</a>
              </td>
              <td class="align-middle">
                <a href="javascript:;" class="text-secondary font-weight-bold text-xs" data-toggle="tooltip"
                  data-original-title="Edit user" @click="ECdiffieHellman(device.device_mac)">ECDH</a>                  
              </td>
              
            </tr>

          </tbody>
        </table>
      </div>
    </div>
  </div>
</template>

<script>
import axios from 'axios';




export default {
  name: "authors-table",
  data() {
    return {
      devices: [],
      conexions: [],
      imageSource: ''
    };
  },
  mounted() {

    this.fetchDeviceData();
    //this.fetchConexionData();

  },
  methods: {
    async fetchDeviceData() {
      try {
        const response = await axios.get('http://127.0.0.1:8000/devices_w_status?limit=20');
        const data = response.data;
        console.log('Device data:', response.data);

        this.devices = data;
        for (let i = 0; i < this.devices.length; i++) {
          console.log('Device:', this.devices[i]['timestamp']);
        }
      } catch (error) {
        console.error('Error fetching device data:', error);
      }
    },
    async fetchConexionData() {
      try {
        const response = await axios.get('http://127.0.0.1:8000/conexions?limit=6');
        const data = response.data;
        console.log('Device data:', response.data);

        this.conexions = data;
        for (let i = 0; i < this.conexions.length; i++) {
          console.log('Status:', this.conexions[i]['online']);
        }
      } catch (error) {
        console.error('Error fetching device data:', error);
      }
    },
    // getImageSource(deviceType) {
    //   console.log('Device type is: ')
    //   console.log(deviceType)
    //   console.log(typeof deviceType)
    //   if (deviceType === "Thermometer") {
    //     this.imageSource = "4346493.png";
    //     console.log(this.imageSource)
    //     return this.imageSource
    //   } else if (deviceType === "Keyboard") {
    //     this.imageSource = "6329376.png";
    //     return this.imageSource
    //   } else if (deviceType === "Screen") {
    //     this.imageSource = "1200px-Simple_Monitor_Icon.svg.png";
        
    //     return this.imageSource
    //   }
    // }
    // ,
    async diffieHellman(deviceMac) {
      try {
        const response = await axios.post('http://127.0.0.1:5050/change-encryption', {
          device_mac : deviceMac,
          encryption_type : 'DH'
        });
        console.log(response.data);
        if (response.status === 200) {
          alert('Encryption type changed to DH');
    }
      } catch (error) {
        console.error(error);
      }
    },
    async ECdiffieHellman(deviceMac) {
      try {
        const response = await axios.post('http://127.0.0.1:5050/change-encryption', {
          device_mac : deviceMac,
          encryption_type : 'ECDH'
        });
        console.log(response.data);
        if (response.status === 200) {
          alert('Encryption type changed to DH');
    }

    // Dismiss the toast notification after 4 seconds
      } catch (error) {
        console.error(error);
      }
    },
    async handleCheckboxChange(deviceMac, event) {
      try {
        const powerState = event.target.checked ? 'on' : 'off';
        const response = await axios.post('http://127.0.0.1:5050/change-device-power', {
          device_mac : deviceMac,
          status : powerState
        });
        console.log(response.data);
  //       if (response.status === 200) {
          
  // }
    } 
       catch (error) {
        console.error(error);
      }
      console.log('Checkbox changed');
    }
  }
};
</script>
<style>
.form-check {
  padding-right : 10px;
}
#x{
  padding-left:90px !important;
}
</style>