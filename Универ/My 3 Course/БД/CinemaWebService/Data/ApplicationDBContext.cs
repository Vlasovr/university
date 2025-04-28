using Microsoft.EntityFrameworkCore;
using CinemaWebService.Models;
using Microsoft.EntityFrameworkCore.Design;

namespace CinemaWebService.Data
{
    public class ApplicationDbContext : DbContext
    {
        public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options)
            : base(options)
        {}

        public DbSet<Client> Clients { get; set; }
        public DbSet<Movie> Movies { get; set; }
        public DbSet<Hall> Halls { get; set; }
        public DbSet<Session> Sessions { get; set; }
        public DbSet<Ticket> Tickets { get; set; }
        public DbSet<Review> Reviews { get; set; }
        public DbSet<Employee> Employees { get; set; }
        public DbSet<Seat> Seats { get; set; }

        public DbSet<SessionEmployee> SessionEmployees { get; set; }
        public DbSet<SessionMovie> SessionMovies { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            base.OnModelCreating(modelBuilder);
            
            modelBuilder.Entity<SessionEmployee>()
                .HasKey(se => new { se.SessionId, se.EmployeeId });
            
            modelBuilder.Entity<SessionMovie>()
                .HasKey(sm => new { sm.SessionId, sm.MovieId });
        }
    }
}

namespace CinemaWebService.Data
{
    public class ApplicationDbContextFactory : IDesignTimeDbContextFactory<ApplicationDbContext>
    {
        public ApplicationDbContext CreateDbContext(string[] args)
        {
            var configuration = new ConfigurationBuilder()
                .SetBasePath(Directory.GetCurrentDirectory())
                .AddJsonFile("appsettings.json")
                .Build();

            var optionsBuilder = new DbContextOptionsBuilder<ApplicationDbContext>();
            var connectionString = configuration.GetConnectionString("MongoDbConnection"); //DefaultConnection for postgres

            optionsBuilder.UseNpgsql(connectionString);

            return new ApplicationDbContext(optionsBuilder.Options);
        }
    }
}
