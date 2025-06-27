// using Microsoft.AspNetCore.Mvc;
// using Microsoft.EntityFrameworkCore;
// using CinemaWebService.Data;
// using CinemaWebService.Models;
//
// namespace CinemaWebService.Controllers
// {
//     [ApiController]
//     [Route("api/[controller]")]
//     public class MoviesController : ControllerBase
//     {
//         private readonly ApplicationDbContext _context;
//
//         public MoviesController(ApplicationDbContext context)
//         {
//             _context = context;
//         }
//         
//         [HttpGet]
//         public async Task<ActionResult<IEnumerable<Movie>>> GetMovies()
//         {
//             return await _context.Movies.ToListAsync();
//         }
//         
//         [HttpGet("{id}")]
//         public async Task<ActionResult<Movie>> GetMovie(int id)
//         {
//             var movie = await _context.Movies
//                 // .Include(m => m.Reviews)
//                 // .Include(m => m.SessionMovies)
//                 .FirstOrDefaultAsync(m => m.MovieId == id);
//
//             if (movie == null)
//             {
//                 return NotFound();
//             }
//
//             return movie;
//         }
//         
//         [HttpPost]
//         public async Task<ActionResult<Movie>> PostMovie(Movie movie)
//         {
//             _context.Movies.Add(movie);
//             await _context.SaveChangesAsync();
//
//             return CreatedAtAction(nameof(GetMovie), new { id = movie.MovieId }, movie);
//         }
//         
//         [HttpPut("{id}")]
//         public async Task<IActionResult> PutMovie(int id, Movie movie)
//         {
//             if (id != movie.MovieId)
//             {
//                 return BadRequest("Идентификаторы не совпадают");
//             }
//
//             _context.Entry(movie).State = EntityState.Modified;
//
//             try
//             {
//                 await _context.SaveChangesAsync();
//             }
//             catch (DbUpdateConcurrencyException)
//             {
//                 if (!MovieExists(id))
//                 {
//                     return NotFound();
//                 }
//                 throw;
//             }
//
//             return NoContent();
//         }
//         
//         [HttpDelete("{id}")]
//         public async Task<IActionResult> DeleteMovie(int id)
//         {
//             var movie = await _context.Movies.FindAsync(id);
//             if (movie == null)
//             {
//                 return NotFound();
//             }
//
//             _context.Movies.Remove(movie);
//             await _context.SaveChangesAsync();
//
//             return NoContent();
//         }
//
//         private bool MovieExists(int id)
//         {
//             return _context.Movies.Any(e => e.MovieId == id);
//         }
//     }
// }


using Microsoft.AspNetCore.Mvc;
using MongoDB.Driver;
using CinemaWebService.Models;
using MongoDB.Driver.Linq;

namespace CinemaWebService.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class MoviesController : ControllerBase
    {
        private readonly IMongoCollection<Movie> _moviesCollection;

        public MoviesController(IMongoDatabase database)
        {
            _moviesCollection = database.GetCollection<Movie>("movies");
        }
        
        [HttpGet]
        public async Task<ActionResult<IEnumerable<Movie>>> GetMovies()
        {
            var movies = await _moviesCollection.Find(_ => true).ToListAsync();
            return Ok(movies);
        }
        
        [HttpGet("{id}")]
        public async Task<ActionResult<Movie>> GetMovie(int id)
        {
            var movie = await _moviesCollection.Find(m => m.MovieId == id).FirstOrDefaultAsync();
            if (movie == null)
            {
                return NotFound();
            }

            return Ok(movie);
        }
        
        [HttpPost]
        public async Task<ActionResult<Movie>> PostMovie(Movie movie)
        {
            // Простейший автоинкремент
            var maxId = await _moviesCollection.AsQueryable().MaxAsync(m => (int?)m.MovieId) ?? 0;
            movie.MovieId = maxId + 1;

            await _moviesCollection.InsertOneAsync(movie);
            return CreatedAtAction(nameof(GetMovie), new { id = movie.MovieId }, movie);
        }
        
        [HttpPut("{id}")]
        public async Task<IActionResult> PutMovie(int id, Movie movie)
        {
            if (id != movie.MovieId)
            {
                return BadRequest("Идентификаторы не совпадают");
            }

            var replaceResult = await _moviesCollection.ReplaceOneAsync(m => m.MovieId == id, movie);
            if (replaceResult.MatchedCount == 0)
            {
                return NotFound();
            }

            return NoContent();
        }
        
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteMovie(int id)
        {
            var deleteResult = await _moviesCollection.DeleteOneAsync(m => m.MovieId == id);
            if (deleteResult.DeletedCount == 0)
            {
                return NotFound();
            }

            return NoContent();
        }
    }
}