<template>
  <main class="main-content">
    <h1 class="main-content__title">Escalonador</h1>
    <div class="main-content__grid">
      <form @submit.prevent="addNewProcess" class="main-form container">
        <div class="main-form__input">
          <label for="name"> Nome do processo : </label>
          <input type="text" v-model="nameOfProcess" placeholder="Digite o nome do processo" />
        </div>

        <div class="main-form__input">
          <label for="priority"> Prioridade : </label>
          <input type="number" v-model="priority" placeholder="Nível de prioridade" />
        </div>

        <div class="main-form__input">
          <label for="time"> Tempo : </label>
          <input type="number" v-model="time" placeholder="Tempo do processo" />
        </div>
        <button type="submit" class="btn"
          :disabled="!(this.nameOfProcess.length > 0 && this.priority > 0 && this.priority <= 5 && this.time > 0)">
          Criar processo
        </button>
        <hr />
      </form>
      <div class="container">
        <button class="btn" @click="biggerPriority">Rodar</button>
        <div class="tasks-grid">
          <div class="task" v-for="task in tasks" :key="task.id">
            <div class="task-content">
              <h4>id :</h4>
              {{ task.id }}
            </div>
            <div class="task-content">
              <h4>Nome processo :</h4>
              {{ task.name }}
            </div>
            <div class="task-content">
              <h4>Tempo :</h4>
              {{ task.time }}
            </div>
            <div class="task-content">
              <h4>Prioridade :</h4>
              {{ task.priority }}
            </div>
            <div v-if="task.running > 0" class="task-content">
              <img src="./assets/img/loader.svg" class="loader" alt="loader pacman" />
            </div>
          </div>
        </div>
      </div>

      <div class="container">

      </div>
    </div>
  </main>
</template>

<script>


export default {
  name: 'App',
  components: {

  },
  data() {
    return {
      nameOfProcess: "",
      priority: 0,
      time: 0,
      tasks: [],
      results: [],
    }
  },
  methods: {
    // Adding a new process to the list of tasks.
    addNewProcess() {
      let task = {
        id: this.tasks.length + 1,
        name: this.nameOfProcess,
        priority: this.priority,
        running: false,
        time: this.time,
        result: 0,
      };
      this.tasks.push(task);
      this.clearForm();
      // this.fairQuee();
    },
    // A method that clears the form.
    clearForm() {
      this.nameOfProcess = "";
      this.priority = 0;
      this.time = 0;
      // this.priority = this.tempo = 1;
    },
    // This method is responsible for updating the total time of the process.
    updateTotalTime(method, total) {
      let med = (total / this.tasks.length).toFixed(2);
      let result = {
        method: method,
        totalTime: total,
        medium: Number(med),
      };
      if (result) {
        this.results.push(result);
      }
    },
    // Sorting the tasks by id and resetting the result to 0.
    restartList() {
      this.tasks.sort((a, b) => {
        console.log("O a ", a);
        return Number(a.id) - Number(b.id);
      });
      this.tasks.forEach((e) => {
        e.result = 0;
      });
    },
    // This method is responsible for calculating the total time of the process.
    fifoAlg(reorder) {
      if (reorder) this.restartList();
      let time = 0;
      let total = 0;
      this.tasks.forEach((t) => {
        time += t.time;
        t.result = time;
        total += t.time;
      });
      if (reorder) {
        this.restartList("FIFO", total);
      } else {
        return total;
      }
      // reorder ? this.restartList('FIFO', total) : total;
    },
    // This method is responsible for sorting the tasks by time and then calculating the total time of
    // the process.
    smallerFirst() {
      this.restartList();
      this.tasks.sort((a, b) => {
        return a.time - b.time;
      });
      // let total = this.fifoAlg(false)
      this.updateTotalTime("SMALLER", this.fifoAlg(false));
    },
    // This method is responsible for sorting the tasks by priority and then calculating the total time of
    // the process.
    biggerPriority() {
      this.restartList();
      this.tasks.sort((a, b) => {
        return b.priority - a.priority;
      });
      this.updateTotalTime("BIGGER", this.fifoAlg(false));
    },
    // This method is responsible for sorting the tasks by time and then calculating the total time of
    // the process.
    circulation() {
      this.restartList();
      let executionRow = this.tasks;
      executionRow.forEach((eR) => {
        eR.delete = false;
      });
      let qt = 0;
      let totalTime = 0;
      let status = true;
      while (status) {
        if (executionRow.length === 0) break;
        executionRow.forEach((eR) => {
          qt++;
          eR.result++;
          if (eR.time == eR.result) {
            this.tasks[eR.id - 1].result = qt;
            eR.delete = true;
          }
        });
        executionRow = executionRow.filter((eR) => !eR.delete);
      }
      this.tasks.forEach((eR) => (totalTime += eR.result));
      this.updateTotalTime("CIRCULATION", totalTime);
    },
    fairQuee() {

      let clone = [];
      console.log("Task 0", this.tasks);
      // let executionTime = Math.floor(Math.random() * 10) * 1000;
      this.tasks.forEach(tk => {
        let task = {
          id: tk.id,
          name: tk.name,
          priority: tk.priority,
          running: false,
          time: tk.time,
          result: 0,

        }
        clone.push(task)
      })
      this.tasks = clone;

      // while (i < this.tasks.length) {

      //   if (this.tasks[i].time > 0) {

      //     setTimeout(() => {
      //       this.tasks[i].running = true;

      //     }, 10)
      //     setTimeout(() => {
      //       this.tasks[i].running = false;
      //       this.tasks[i].time -= executionTime;
      //     }, executionTime)
      //     i++;
      //   }

      // }

    },
    // This method is responsible for calling all the methods that calculate the total time of the process.
    callAll() {
      this.tasks = [];
      this.fifoAlg(true);
      this.smallerFirst();
      this.biggerPriority();
      this.circulation();
      this.results.sort((a, b) => {
        return a.medium - b.medium;
      });
    },
  },
}
</script>

<style>
@import url("https://fonts.googleapis.com/css2?family=Oswald:wght@200;300;400;500;600;700&display=swap");

:root {
  --primary-color: #f8f8f8;
}

* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

a {
  text-decoration: none;
}

li {
  list-style: none;
}

html {
  font-size: 62.5%;
}

body {
  margin: 0 auto;
  background-color: #d5d96f;
  background-image: url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 304 304' width='304' height='304'%3E%3Cpath fill='%23727597' fill-opacity='0.73' d='M44.1 224a5 5 0 1 1 0 2H0v-2h44.1zm160 48a5 5 0 1 1 0 2H82v-2h122.1zm57.8-46a5 5 0 1 1 0-2H304v2h-42.1zm0 16a5 5 0 1 1 0-2H304v2h-42.1zm6.2-114a5 5 0 1 1 0 2h-86.2a5 5 0 1 1 0-2h86.2zm-256-48a5 5 0 1 1 0 2H0v-2h12.1zm185.8 34a5 5 0 1 1 0-2h86.2a5 5 0 1 1 0 2h-86.2zM258 12.1a5 5 0 1 1-2 0V0h2v12.1zm-64 208a5 5 0 1 1-2 0v-54.2a5 5 0 1 1 2 0v54.2zm48-198.2V80h62v2h-64V21.9a5 5 0 1 1 2 0zm16 16V64h46v2h-48V37.9a5 5 0 1 1 2 0zm-128 96V208h16v12.1a5 5 0 1 1-2 0V210h-16v-76.1a5 5 0 1 1 2 0zm-5.9-21.9a5 5 0 1 1 0 2H114v48H85.9a5 5 0 1 1 0-2H112v-48h12.1zm-6.2 130a5 5 0 1 1 0-2H176v-74.1a5 5 0 1 1 2 0V242h-60.1zm-16-64a5 5 0 1 1 0-2H114v48h10.1a5 5 0 1 1 0 2H112v-48h-10.1zM66 284.1a5 5 0 1 1-2 0V274H50v30h-2v-32h18v12.1zM236.1 176a5 5 0 1 1 0 2H226v94h48v32h-2v-30h-48v-98h12.1zm25.8-30a5 5 0 1 1 0-2H274v44.1a5 5 0 1 1-2 0V146h-10.1zm-64 96a5 5 0 1 1 0-2H208v-80h16v-14h-42.1a5 5 0 1 1 0-2H226v18h-16v80h-12.1zm86.2-210a5 5 0 1 1 0 2H272V0h2v32h10.1zM98 101.9V146H53.9a5 5 0 1 1 0-2H96v-42.1a5 5 0 1 1 2 0zM53.9 34a5 5 0 1 1 0-2H80V0h2v34H53.9zm60.1 3.9V66H82v64H69.9a5 5 0 1 1 0-2H80V64h32V37.9a5 5 0 1 1 2 0zM101.9 82a5 5 0 1 1 0-2H128V37.9a5 5 0 1 1 2 0V82h-28.1zm16-64a5 5 0 1 1 0-2H146v44.1a5 5 0 1 1-2 0V18h-26.1zm102.2 270a5 5 0 1 1 0 2H98v14h-2v-16h124.1zM242 149.9V160h16v34h-16v62h48v48h-2v-46h-48v-66h16v-30h-16v-12.1a5 5 0 1 1 2 0zM53.9 18a5 5 0 1 1 0-2H64V2H48V0h18v18H53.9zm112 32a5 5 0 1 1 0-2H192V0h50v2h-48v48h-28.1zm-48-48a5 5 0 0 1-9.8-2h2.07a3 3 0 1 0 5.66 0H178v34h-18V21.9a5 5 0 1 1 2 0V32h14V2h-58.1zm0 96a5 5 0 1 1 0-2H137l32-32h39V21.9a5 5 0 1 1 2 0V66h-40.17l-32 32H117.9zm28.1 90.1a5 5 0 1 1-2 0v-76.51L175.59 80H224V21.9a5 5 0 1 1 2 0V82h-49.59L146 112.41v75.69zm16 32a5 5 0 1 1-2 0v-99.51L184.59 96H300.1a5 5 0 0 1 3.9-3.9v2.07a3 3 0 0 0 0 5.66v2.07a5 5 0 0 1-3.9-3.9H185.41L162 121.41v98.69zm-144-64a5 5 0 1 1-2 0v-3.51l48-48V48h32V0h2v50H66v55.41l-48 48v2.69zM50 53.9v43.51l-48 48V208h26.1a5 5 0 1 1 0 2H0v-65.41l48-48V53.9a5 5 0 1 1 2 0zm-16 16V89.41l-34 34v-2.82l32-32V69.9a5 5 0 1 1 2 0zM12.1 32a5 5 0 1 1 0 2H9.41L0 43.41V40.6L8.59 32h3.51zm265.8 18a5 5 0 1 1 0-2h18.69l7.41-7.41v2.82L297.41 50H277.9zm-16 160a5 5 0 1 1 0-2H288v-71.41l16-16v2.82l-14 14V210h-28.1zm-208 32a5 5 0 1 1 0-2H64v-22.59L40.59 194H21.9a5 5 0 1 1 0-2H41.41L66 216.59V242H53.9zm150.2 14a5 5 0 1 1 0 2H96v-56.6L56.6 162H37.9a5 5 0 1 1 0-2h19.5L98 200.6V256h106.1zm-150.2 2a5 5 0 1 1 0-2H80v-46.59L48.59 178H21.9a5 5 0 1 1 0-2H49.41L82 208.59V258H53.9zM34 39.8v1.61L9.41 66H0v-2h8.59L32 40.59V0h2v39.8zM2 300.1a5 5 0 0 1 3.9 3.9H3.83A3 3 0 0 0 0 302.17V256h18v48h-2v-46H2v42.1zM34 241v63h-2v-62H0v-2h34v1zM17 18H0v-2h16V0h2v18h-1zm273-2h14v2h-16V0h2v16zm-32 273v15h-2v-14h-14v14h-2v-16h18v1zM0 92.1A5.02 5.02 0 0 1 6 97a5 5 0 0 1-6 4.9v-2.07a3 3 0 1 0 0-5.66V92.1zM80 272h2v32h-2v-32zm37.9 32h-2.07a3 3 0 0 0-5.66 0h-2.07a5 5 0 0 1 9.8 0zM5.9 0A5.02 5.02 0 0 1 0 5.9V3.83A3 3 0 0 0 3.83 0H5.9zm294.2 0h2.07A3 3 0 0 0 304 3.83V5.9a5 5 0 0 1-3.9-5.9zm3.9 300.1v2.07a3 3 0 0 0-1.83 1.83h-2.07a5 5 0 0 1 3.9-3.9zM97 100a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0-16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm-48 32a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm32 48a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm-16 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm32-16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0-32a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16 32a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm32 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0-16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm-16-64a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16 0a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16 96a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16-144a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0 32a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16-32a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16-16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm-96 0a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16-32a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm96 0a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm-16-64a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16-16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm-32 0a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0-16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm-16 0a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm-16 0a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm-16 0a3 3 0 1 0 0-6 3 3 0 0 0 0 6zM49 36a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm-32 0a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm32 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zM33 68a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16-48a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0 240a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16 32a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm-16-64a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm-16-32a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm80-176a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16 0a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm-16-16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm32 48a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16-16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0-32a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm112 176a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm-16 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zM17 180a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0 16a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm0-32a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16 0a3 3 0 1 0 0-6 3 3 0 0 0 0 6zM17 84a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm32 64a3 3 0 1 0 0-6 3 3 0 0 0 0 6zm16-16a3 3 0 1 0 0-6 3 3 0 0 0 0 6z'%3E%3C/path%3E%3C/svg%3E");
  font-size: 1.6rem;
  font-family: "Oswald", sans-serif;
}

.main-content {

  text-align: center;
  margin-top: 2rem;
  display: flex;
  align-content: center;
  align-items: center;
  flex-direction: column;
}

.main-content__title {
  font-size: 4rem;
  text-transform: uppercase;
  font-weight: 300;
}

.loader {

  height: 4rem;
  width: 4rem;
}

.main-form {
  border-radius: 1rem;
  padding: 5rem;
  display: flex;
  align-items: center;
  gap: 2rem;
  justify-content: center;
  flex-direction: column;
  min-height: 40rem;
  max-width: 40rem;
  background: #ccc;
  background-color: rgba(255, 255, 255, 0.4);
  box-shadow: 0 10px 10px 10px rgba(0, 0, 0, 0.2);
}

.main-form__input {
  display: flex;
  align-items: center;
  gap: 1rem;
  font-size: 2.2rem;

  background-color: #333;
  color: #fff;
  max-height: 30rem;
  padding: 1rem;
  border-radius: 0.5rem;
  flex-flow: column wrap;
}

.main-form__input input {
  border: none;
  padding: 1rem;
  border-radius: 0.2rem;
}

.main-form .btn {
  cursor: pointer;
  font-size: 2rem;
  height: 4rem;

  border: none;
  border-radius: 10px;
  color: #fff;

  background-color: #333;
  outline: none;
  padding: 0.5rem 1.8rem;
  /* transition: 1s ease-in-out; */
  box-shadow: 0 0.3rem rgba(121, 121, 121, 0.65);
}

.main-form .btn:disabled {
  background: #333c;
}

.main-content .main-content__grid {
  display: grid;
  grid-template-columns: 1fr 1fr;
  grid-template-rows: 1fr;
  gap: 5rem;
}

.container {
  border-radius: 1rem;
  padding: 1rem;
  min-height: 50rem;
  min-width: 34.3rem;
  background: #ccc;
  background-color: rgba(255, 255, 255, 0.4);
  box-shadow: 0 10px 10px 10px rgba(0, 0, 0, 0.2);
}

.tasks-grid {
  display: grid;
  grid-template-columns: 1fr;
  gap: 1rem;
  max-height: 50rem;
  overflow-y: auto;
  position: relative;
  grid-template-rows: repeat(auto, 1fr);
}

.task-column {

  margin: 1rem;
  display: flex;
  max-height: 50rem;
  align-items: center;
  gap: 0.2rem;
  justify-content: center;
  flex-direction: column;
  overflow-y: auto;
  position: relative;
}

.task {
  background: #333;

  color: #fff;
  padding: 0.5rem;
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  grid-template-rows: repeat(1, 1fr);

  border-radius: 0.5rem;
}

.task .task-content {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
}
</style>
