// using Microsoft.AspNetCore.Mvc;
// using Microsoft.EntityFrameworkCore;
// using CinemaWebService.Data;
// using CinemaWebService.Models;
//
// namespace CinemaWebService.Controllers
// {
//     [ApiController]
//     [Route("api/[controller]")]
//     public class ReviewsController : ControllerBase
//     {
//         private readonly ApplicationDbContext _context;
//
//         public ReviewsController(ApplicationDbContext context)
//         {
//             _context = context;
//         }
//
//         // GET: api/Reviews
//         [HttpGet]
//         public async Task<ActionResult<IEnumerable<Review>>> GetReviews()
//         {
//             return await _context.Reviews
//                 // .Include(r => r.Movie)
//                 // .Include(r => r.Client)
//                 .ToListAsync();
//         }
//
//         // GET: api/Reviews/5
//         [HttpGet("{id}")]
//         public async Task<ActionResult<Review>> GetReview(int id)
//         {
//             var review = await _context.Reviews
//                 // .Include(r => r.Movie)
//                 // .Include(r => r.Client)
//                 .FirstOrDefaultAsync(r => r.ReviewId == id);
//
//             if (review == null)
//             {
//                 return NotFound();
//             }
//
//             return review;
//         }
//
//         // POST: api/Reviews
//         [HttpPost]
//         public async Task<ActionResult<Review>> PostReview(Review review)
//         {
//             _context.Reviews.Add(review);
//             await _context.SaveChangesAsync();
//
//             return CreatedAtAction(nameof(GetReview), new { id = review.ReviewId }, review);
//         }
//
//         // PUT: api/Reviews/5
//         [HttpPut("{id}")]
//         public async Task<IActionResult> PutReview(int id, Review review)
//         {
//             if (id != review.ReviewId)
//             {
//                 return BadRequest("Идентификаторы не совпадают");
//             }
//
//             _context.Entry(review).State = EntityState.Modified;
//
//             try
//             {
//                 await _context.SaveChangesAsync();
//             }
//             catch (DbUpdateConcurrencyException)
//             {
//                 if (!ReviewExists(id))
//                 {
//                     return NotFound();
//                 }
//                 throw;
//             }
//
//             return NoContent();
//         }
//
//         // DELETE: api/Reviews/5
//         [HttpDelete("{id}")]
//         public async Task<IActionResult> DeleteReview(int id)
//         {
//             var review = await _context.Reviews.FindAsync(id);
//             if (review == null)
//             {
//                 return NotFound();
//             }
//
//             _context.Reviews.Remove(review);
//             await _context.SaveChangesAsync();
//
//             return NoContent();
//         }
//
//         private bool ReviewExists(int id)
//         {
//             return _context.Reviews.Any(e => e.ReviewId == id);
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
    public class ReviewsController : ControllerBase
    {
        private readonly IMongoCollection<Review> _reviewsCollection;

        public ReviewsController(IMongoDatabase database)
        {
            _reviewsCollection = database.GetCollection<Review>("reviews");
        }

        // GET: api/Reviews
        [HttpGet]
        public async Task<ActionResult<IEnumerable<Review>>> GetReviews()
        {
            var reviews = await _reviewsCollection.Find(_ => true).ToListAsync();
            return Ok(reviews);
        }

        // GET: api/Reviews/5
        [HttpGet("{id}")]
        public async Task<ActionResult<Review>> GetReview(int id)
        {
            var review = await _reviewsCollection.Find(r => r.ReviewId == id).FirstOrDefaultAsync();
            if (review == null)
            {
                return NotFound();
            }

            return Ok(review);
        }

        // POST: api/Reviews
        [HttpPost]
        public async Task<ActionResult<Review>> PostReview(Review review)
        {
            // Простейший вариант автоинкремента
            var maxId = await _reviewsCollection.AsQueryable().MaxAsync(r => (int?)r.ReviewId) ?? 0;
            review.ReviewId = maxId + 1;

            await _reviewsCollection.InsertOneAsync(review);
            return CreatedAtAction(nameof(GetReview), new { id = review.ReviewId }, review);
        }

        // PUT: api/Reviews/5
        [HttpPut("{id}")]
        public async Task<IActionResult> PutReview(int id, Review review)
        {
            if (id != review.ReviewId)
            {
                return BadRequest("Идентификаторы не совпадают");
            }

            var replaceResult = await _reviewsCollection.ReplaceOneAsync(r => r.ReviewId == id, review);
            if (replaceResult.MatchedCount == 0)
            {
                return NotFound();
            }

            return NoContent();
        }

        // DELETE: api/Reviews/5
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteReview(int id)
        {
            var deleteResult = await _reviewsCollection.DeleteOneAsync(r => r.ReviewId == id);
            if (deleteResult.DeletedCount == 0)
            {
                return NotFound();
            }

            return NoContent();
        }
    }
}