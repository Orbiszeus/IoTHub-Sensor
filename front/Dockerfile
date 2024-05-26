# FROM node:lts-alpine as build-stage
# WORKDIR /app
# COPY package*.json ./
# RUN npm install
# COPY . .
# RUN npm run build

# FROM nginx:stable-alpine as production-stage
# COPY --from=build-stage /usr/app /usr/share/nginx/html
# EXPOSE 80
# CMD ["npm", "run", "serve"]

# NPM manager
FROM node:16-alpine3.12
RUN npm install -g @vue/cli
RUN mkdir /front
COPY package*.json ./
RUN npm install
WORKDIR /front
COPY . /front
CMD [ "npm", "run", "serve" ]