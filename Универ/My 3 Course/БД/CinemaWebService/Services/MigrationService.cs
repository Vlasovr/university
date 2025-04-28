using CinemaWebService.Data;
using CinemaWebService.Models;
using Microsoft.Extensions.Configuration;
using MongoDB.Driver;

namespace CinemaWebService.Services
{
    public class MigrationService
    {
        private readonly ApplicationDbContext _dbContext;
        
        private readonly IMongoCollection<Client> _clientsCollection;
        private readonly IMongoCollection<Employee> _employeesCollection;
        private readonly IMongoCollection<Hall> _hallsCollection;
        private readonly IMongoCollection<Movie> _moviesCollection;
        private readonly IMongoCollection<Review> _reviewsCollection;
        private readonly IMongoCollection<Seat> _seatsCollection;
        private readonly IMongoCollection<Session> _sessionsCollection;
        private readonly IMongoCollection<SessionEmployee> _sessionEmployeesCollection;
        private readonly IMongoCollection<SessionMovie> _sessionMoviesCollection;
        private readonly IMongoCollection<Ticket> _ticketsCollection;

        public MigrationService(ApplicationDbContext dbContext, IConfiguration configuration)
        {
            _dbContext = dbContext;
            
            var mongoConnectionString = configuration.GetConnectionString("MongoDbConnection");
            var mongoClient = new MongoClient(mongoConnectionString);
            
            var mongoDatabase = mongoClient.GetDatabase("cinema_mongo_db");
            
            _clientsCollection         = mongoDatabase.GetCollection<Client>("clients");
            _employeesCollection       = mongoDatabase.GetCollection<Employee>("employees");
            _hallsCollection           = mongoDatabase.GetCollection<Hall>("halls");
            _moviesCollection          = mongoDatabase.GetCollection<Movie>("movies");
            _reviewsCollection         = mongoDatabase.GetCollection<Review>("reviews");
            _seatsCollection           = mongoDatabase.GetCollection<Seat>("seats");
            _sessionsCollection        = mongoDatabase.GetCollection<Session>("sessions");
            _sessionEmployeesCollection= mongoDatabase.GetCollection<SessionEmployee>("sessionEmployees");
            _sessionMoviesCollection   = mongoDatabase.GetCollection<SessionMovie>("sessionMovies");
            _ticketsCollection         = mongoDatabase.GetCollection<Ticket>("tickets");
        }
        
        public async Task MigrateAllAsync()
        {
            var clients = _dbContext.Clients.ToList();
            if (clients.Any())
                await _clientsCollection.InsertManyAsync(clients);
            
            var employees = _dbContext.Employees.ToList();
            if (employees.Any())
                await _employeesCollection.InsertManyAsync(employees);
            
            var halls = _dbContext.Halls.ToList();
            if (halls.Any())
                await _hallsCollection.InsertManyAsync(halls);
            
            var movies = _dbContext.Movies.ToList();
            if (movies.Any())
                await _moviesCollection.InsertManyAsync(movies);
            
            var reviews = _dbContext.Reviews.ToList();
            if (reviews.Any())
                await _reviewsCollection.InsertManyAsync(reviews);
            
            var seats = _dbContext.Seats.ToList();
            if (seats.Any())
                await _seatsCollection.InsertManyAsync(seats);
            
            var sessions = _dbContext.Sessions.ToList();
            if (sessions.Any())
                await _sessionsCollection.InsertManyAsync(sessions);
            
            var sessionEmployees = _dbContext.SessionEmployees.ToList();
            if (sessionEmployees.Any())
                await _sessionEmployeesCollection.InsertManyAsync(sessionEmployees);
            
            var sessionMovies = _dbContext.SessionMovies.ToList();
            if (sessionMovies.Any())
                await _sessionMoviesCollection.InsertManyAsync(sessionMovies);
            
            var tickets = _dbContext.Tickets.ToList();
            if (tickets.Any())
                await _ticketsCollection.InsertManyAsync(tickets);
        }
    }
}